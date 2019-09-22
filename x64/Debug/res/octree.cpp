using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

using EricsLib.ContentDatabases;
using EricsLib.Geometries;
using EricsLib.Engine;
using EricsLib.MathLib;
using EricsLib.IO;
using EricsLib.PrimitiveSystem;

namespace EricsLib.Datastructures
{
    /// <summary>
    /// The Oct Tree is a 3 Dimensional version of the Quad Tree. It breaks up a 3D space into enclosing boxes.
    /// Partitioning logic: If a node contains two or more objects, partition the node by subdividing it into 8 quadrants. 
    /// Then, try to place all objects into the subdivided regions of space. If the objects cannot be fully contained, leave them in their current node.
    /// This is implemented recursively.
    /// </summary>
    /// <remarks>
    /// Usage:
    /// 1. use the SimpleAdd() to put items into the octree collection
    /// 2. Call the SimpleUpdate() to update all objects in the octree collection. The tree will be built at this time based on all collection items.
    /// 3. Use any colliders at any time to get a list of intersections
    /// </remarks>
    public class OctTree
    {
        #region Fields

        /// <summary>
        /// This is the bounding box region which contains all objects within the tree node.
        /// </summary>
        BoundingBox m_region;

        /// <summary>
        /// This is a list of all objects within the current node of the octree.
        /// </summary>
        List<Physical> m_objects;

        /// <summary>
        /// These are items which we're waiting to insert into the data structure. 
        /// We want to accrue as many objects in here as possible before we inject them into the tree. This is slightly more cache friendly.
        /// </summary>
        static Queue<Physical> m_pendingInsertion = new Queue<Physical>();

        /// <summary>
        /// This is a global list of all the objects within the octree, for easy reference.
        /// </summary>
        static List<Physical> m_allObjects = new List<Physical>();

        static List<Physical> m_deadObjects = new List<Physical>();

        /// <summary>
        /// These are all of the possible child octants for this node in the tree.
        /// </summary>
        OctTree[] m_childNode = new OctTree[8];

        /// <summary>
        /// This is a bit mask indicating which child nodes are actively being used.
        /// It adds slightly more complexity, but is faster for performance since there is only one comparison instead of 8.
        /// </summary>
        byte m_activeNodes = 0;

        /// <summary>
        /// The minimum size for enclosing region is a 1x1x1 cube.
        /// </summary>
        const int MIN_SIZE = 1;

        /// <summary>
        /// this is how many frames we'll wait before deleting an empty tree branch. Note that this is not a constant. The maximum lifespan doubles
        /// every time a node is reused, until it hits a hard coded constant of 64
        /// </summary>
        int m_maxLifespan = 8;          //
        int m_curLife = -1;             //this is how much time we have left

        /// <summary>
        /// A reference to the parent node is sometimes required. If we are a node and we realize that we no longer have items contained within ourselves,
        /// we need to let our parent know that we're empty so that it can delete us.
        /// </summary>
        OctTree _parent;
        
        static bool m_treeReady = false;       //the tree has a few objects which need to be inserted before it is complete
        static bool m_treeBuilt = false;       //there is no pre-existing tree yet.

        /// <summary>
        /// This is a reference to the root node of the octree.
        /// </summary>
        static OctTree m_root;

        static LineCollection m_lineCollection;// = new LineCollection();
        Color m_lineColor = Color.Lime;
        #endregion

        #region Constructors & Unloaders
        /*Note: we want to avoid allocating memory for as long as possible since there can be lots of nodes.*/
        /// <summary>
        /// Creates an oct tree which encloses the given region and contains the provided objects.
        /// </summary>
        /// <param name="region">The bounding region for the oct tree.</param>
        /// <param name="objList">The list of objects contained within the bounding region</param>
        private OctTree(BoundingBox region, List<Physical> objList)
        {
            m_region = region;
            m_objects = objList;
            m_curLife = -1;

            //CollectionSettings tmpSettings = new CollectionSettings();
            //tmpSettings.AssetDB = BaseSettings.AssetDB;
            //tmpSettings.GraphicsDevice = BaseSettings.Graphics;
            //tmpSettings.BlendState = BlendState.AlphaBlend;
            //m_lineCollection.Initialize(tmpSettings);
            //BaseSettings.Primitives.AddLineCollection(m_lineCollection);
        }

        /// <summary>
        /// Creates an Octree which is ready for object insertion. The dimensions of the octree will scale to enclose all inserted objects.
        /// </summary>
        public OctTree()
        {
            m_objects = new List<Physical>();
            m_region = new BoundingBox(Vector3.Zero, Vector3.Zero);
            m_curLife = -1;

            //CollectionSettings tmpSettings = new CollectionSettings();
            //tmpSettings.AssetDB = BaseSettings.AssetDB;
            //tmpSettings.GraphicsDevice = BaseSettings.Graphics;
            //tmpSettings.BlendState = BlendState.AlphaBlend;
            //m_lineCollection.Initialize(tmpSettings);
            //BaseSettings.Primitives.AddLineCollection(m_lineCollection);
        }

        /// <summary>
        /// Creates an octTree with a suggestion for the bounding region containing the items.
        /// </summary>
        /// <param name="region">The suggested dimensions for the bounding region. 
        /// Note: if items are outside this region, the region will be automatically resized.</param>
        public OctTree(BoundingBox region)
        {
            m_region = region;
            m_objects = new List<Physical>();
            m_curLife = -1;

            //CollectionSettings tmpSettings = new CollectionSettings();
            //tmpSettings.AssetDB = BaseSettings.AssetDB;
            //tmpSettings.GraphicsDevice = BaseSettings.Graphics;
            //tmpSettings.BlendState = BlendState.AlphaBlend;
            //m_lineCollection.Initialize(tmpSettings);
            //BaseSettings.Primitives.AddLineCollection(m_lineCollection);
        }

        /// <summary>
        /// Completely removes all content from the octree
        /// </summary>
        public void UnloadContent()
        {
            m_pendingInsertion.Clear();
            UnloadHelper(this);
            m_treeBuilt = false;
            m_treeReady = false;
        }

        /// <summary>
        /// Recursive helper function for removing all nodes in the tree
        /// </summary>
        /// <param name="root">The root node to start deleting from</param>
        private void UnloadHelper(OctTree root)
        {
            if (root == null) return;

            if (root.m_objects != null)
            {
                root.m_objects.Clear();
                //root.m_objects = null;
            }

            if (m_region != null)
            {
                root.m_region.Max = Vector3.Zero;
                root.m_region.Min = Vector3.Zero;
            }

            if (root.m_childNode != null && root.m_activeNodes != 0)
            {
                for (int a = 0; a < 8; a++)
                {
                    if (root.m_childNode[a] != null)
                    {
                        root.UnloadHelper(root.m_childNode[a]);
                        root.m_childNode[a] = null;
                    }
                }
            }
            m_activeNodes = 0;
            //root.m_childNode = null;
            root._parent = null;

        }
        #endregion

        #region Rendering for debug

        public void RenderReset()
        {
            m_lineCollection.Clear();
        }
        /// <summary>
        /// Renders the current state of the octTree by drawing the outlines of each bounding region.
        /// </summary>
        /// <param name="pb">The primitive batch being used to draw the OctTree.</param>
        public void Render(PrimitiveSystem.PrimitiveSystem pb)
        {
            Vector3[] verts = new Vector3[8];
            verts[0] = m_region.Min;
            verts[1] = new Vector3(m_region.Min.X, m_region.Min.Y, m_region.Max.Z); //Z
            verts[2] = new Vector3(m_region.Min.X, m_region.Max.Y, m_region.Min.Z); //Y
            verts[3] = new Vector3(m_region.Max.X, m_region.Min.Y, m_region.Min.Z); //X

            verts[7] = m_region.Max;
            verts[4] = new Vector3(m_region.Max.X, m_region.Max.Y, m_region.Min.Z); //Z
            verts[5] = new Vector3(m_region.Max.X, m_region.Min.Y, m_region.Max.Z); //Y
            verts[6] = new Vector3(m_region.Min.X, m_region.Max.Y, m_region.Max.Z); //X





            m_lineCollection.AddLineSegment(verts[0], verts[1], m_lineColor);
            m_lineCollection.AddLineSegment(verts[0], verts[2], m_lineColor);
            m_lineCollection.AddLineSegment(verts[0], verts[3], m_lineColor);
            m_lineCollection.AddLineSegment(verts[7], verts[4], m_lineColor);
            m_lineCollection.AddLineSegment(verts[7], verts[5], m_lineColor);
            m_lineCollection.AddLineSegment(verts[7], verts[6], m_lineColor);

            m_lineCollection.AddLineSegment(verts[1], verts[6], m_lineColor);
            m_lineCollection.AddLineSegment(verts[1], verts[5], m_lineColor);
            m_lineCollection.AddLineSegment(verts[4], verts[2], m_lineColor);
            m_lineCollection.AddLineSegment(verts[4], verts[3], m_lineColor);
            m_lineCollection.AddLineSegment(verts[2], verts[6], m_lineColor);
            m_lineCollection.AddLineSegment(verts[3], verts[5], m_lineColor);

            //pb.AddLine(new VertexPositionColor(verts[0], Color.Lime), new VertexPositionColor(verts[1], Color.Lime));
            //pb.AddLine(new VertexPositionColor(verts[0], Color.Lime), new VertexPositionColor(verts[2], Color.Lime));
            //pb.AddLine(new VertexPositionColor(verts[0], Color.Lime), new VertexPositionColor(verts[3], Color.Lime));
            //pb.AddLine(new VertexPositionColor(verts[7], Color.Lime), new VertexPositionColor(verts[4], Color.Lime));
            //pb.AddLine(new VertexPositionColor(verts[7], Color.Lime), new VertexPositionColor(verts[5], Color.Lime));
            //pb.AddLine(new VertexPositionColor(verts[7], Color.Lime), new VertexPositionColor(verts[6], Color.Lime));

            //pb.AddLine(new VertexPositionColor(verts[1], Color.Lime), new VertexPositionColor(verts[6], Color.Lime));
            //pb.AddLine(new VertexPositionColor(verts[1], Color.Lime), new VertexPositionColor(verts[5], Color.Lime));
            //pb.AddLine(new VertexPositionColor(verts[4], Color.Lime), new VertexPositionColor(verts[2], Color.Lime));
            //pb.AddLine(new VertexPositionColor(verts[4], Color.Lime), new VertexPositionColor(verts[3], Color.Lime));
            //pb.AddLine(new VertexPositionColor(verts[2], Color.Lime), new VertexPositionColor(verts[6], Color.Lime));
            //pb.AddLine(new VertexPositionColor(verts[3], Color.Lime), new VertexPositionColor(verts[5], Color.Lime));

            

            for (int a = 0; a < 8; a++)
            {
                if (m_childNode[a] != null)
                    m_childNode[a].Render(pb);
            }
        }

        public void DrawDebug(OctTree currentNode)
        {
            //top left to top right
            BaseSettings.SpriteBatch.Draw(BaseSettings.AssetDB.GetTexture("Solid"), new Rectangle((int)currentNode.m_region.Min.X, (int)currentNode.m_region.Min.Y, (int)(currentNode.m_region.Max.X - currentNode.m_region.Min.X), 1), Color.Green);

            //bottom left to bottom right
            BaseSettings.SpriteBatch.Draw(BaseSettings.AssetDB.GetTexture("Solid"), new Rectangle((int)currentNode.m_region.Min.X, (int)currentNode.m_region.Max.Y, (int)(currentNode.m_region.Max.X - currentNode.m_region.Min.X), 1), Color.Green);

            //top left to bottom left
            BaseSettings.SpriteBatch.Draw(BaseSettings.AssetDB.GetTexture("Solid"), new Rectangle((int)currentNode.m_region.Min.X, (int)currentNode.m_region.Min.Y, 1, (int)(currentNode.m_region.Max.Y - currentNode.m_region.Min.Y)), Color.Green);

            //top right to bottom right
            BaseSettings.SpriteBatch.Draw(BaseSettings.AssetDB.GetTexture("Solid"), new Rectangle((int)currentNode.m_region.Max.X, (int)currentNode.m_region.Min.Y, 1, (int)(currentNode.m_region.Max.Y - currentNode.m_region.Min.Y)), Color.Green);
            
            if (HasChildren)
            {
                for (int a = 0; a < 8; a++)
                {
                    if (currentNode.m_childNode != null && currentNode.m_childNode[a] != null)
                        currentNode.m_childNode[a].DrawDebug(currentNode.m_childNode[a]);
                }
            }
        }

        /// <summary>
        /// Draws all objects contained in the octree collection
        /// </summary>
        /// <param name="time">The game time</param>
        public void Draw(coreTime time)
        {
            foreach (Physical obj in m_allObjects)
            {
                obj.Draw(time);
            }
        }
        #endregion

        #region Updates
        public void UpdateObjects(coreTime time)
        {
            //handle all pending object insertions, movements, tree construction, etc.

            if (m_pendingInsertion.Count > 0)
                ProcessPendingItems();

            UpdateTreeObjects(this, time);

            //UpdateTree(movedObjects);

            PruneDeadBranches(this);

        }

        /// <summary>
        /// This updates the structure of the tree based on the position of all moved objects.
        /// </summary>
        private void UpdateTree(List<Physical> movedObjects)
        {
            

            if (m_treeBuilt == true)
            {
                if (movedObjects.Count > 0)
                {
                    //objects have moved! update the tree structure
                    foreach (Physical movedObject in movedObjects)
                    {
                        //grab the node which this object *should* occupy
                        OctTree tgtNode = FindContainingChildnode(movedObject, m_root);
                        if (tgtNode == null)
                            FindContainingChildnode(movedObject, m_root);

                        //grab the node which this object currently occupies
                        OctTree curNode = FindObjectInTree(movedObject, m_root);

                        if (tgtNode != curNode)
                        {
                            curNode.Remove(movedObject);
                            bool attempt = tgtNode.Insert(movedObject);
                            if (!attempt)
                            {
                                //DEBUG
                                bool result = tgtNode.Insert(movedObject);

                                List<Physical> tmp = m_root.AllObjects();
                                UnloadContent();
                                m_root = new OctTree(new BoundingBox(Vector3.Zero, Vector3.Zero));
                                Enqueue(tmp);
                                ProcessPendingItems();

                                //m_root.UnloadContent();
                                //Enqueue(tmp);//add to pending queue
                                //m_root.m_objects.AddRange(tmp);

                                //m_objects.Add(Item);
                                //BuildTree();
                            }

                            //the object no longer fits in the tree
                            //if (curNode != null)
                            //{
                            //    curNode.m_objects.Remove(movedObject);
                            //}
                            //else
                            //{
                            //}
                        }
                    }

                    
                }
            }
            else
            {
                //build the tree
            }
        }

        public void Update(coreTime time)
        {
            if (m_treeBuilt == true && m_treeReady == true)
            {
                //Start a count down death timer for any leaf nodes which don't have objects or children.
                //when the timer reaches zero, we delete the leaf. If the node is reused before death, we double its lifespan.
                //this gives us a "frequency" usage score and lets us avoid allocating and deallocating memory unnecessarily
                if (m_objects.Count == 0)
                {
                    if (HasChildren == false)
                    {
                        if (m_curLife == -1)
                            m_curLife = m_maxLifespan;
                        else if (m_curLife > 0)
                        {
                            m_curLife--;
                        }
                    }
                }
                else
                {
                    if (m_curLife != -1)
                    {
                        if (m_maxLifespan <= 64)
                            m_maxLifespan *= 2;
                        m_curLife = -1;
                    }
                }

                List<Physical> movedObjects = new List<Physical>(m_objects.Count);

                //go through and update every object in the current tree node
                foreach (Physical gameObj in m_objects)
                {
                    //we should figure out if an object actually moved so that we know whether we need to update this node in the tree.
                    if (gameObj.Update(time) == 1)
                    {
                        movedObjects.Add(gameObj);
                    }
                }

                //prune any dead objects from the tree.
                int listSize = m_objects.Count;
                for (int a = 0; a < listSize; a++)
                {
                    if (!m_objects[a].Alive)
                    {
                        if (movedObjects.Contains(m_objects[a]))
                            movedObjects.Remove(m_objects[a]);
                        m_objects.RemoveAt(a--);
                        listSize--;
                    }
                }

                //prune out any dead branches in the tree
                for (int flags = m_activeNodes, index = 0; flags > 0; flags >>= 1, index++)
                    if ((flags & 1) == 1 && m_childNode[index].m_curLife == 0)
                    {
                        if (m_childNode[index].m_objects.Count > 0)
                        {
                            //throw new Exception("Tried to delete a used branch!");
                            m_childNode[index].m_curLife = -1;
                        }
                        else
                        {
                            m_childNode[index] = null;
                            m_activeNodes ^= (byte)(1 << index);       //remove the node from the active nodes flag list
                        }
                    }

                //recursively update any child nodes.
                for (int flags = m_activeNodes, index = 0; flags > 0; flags >>= 1, index++)
                {
                    if ((flags & 1) == 1)
                    {
                        if(m_childNode!=null && m_childNode[index] != null)
                            m_childNode[index].Update(time);
                    }
                }



                //If an object moved, we can insert it into the parent and that will insert it into the correct tree node.
                //note that we have to do this last so that we don't accidentally update the same object more than once per frame.
                foreach (Physical movedObj in movedObjects)
                {
                    OctTree current = this;
                    

                    //figure out how far up the tree we need to go to reinsert our moved object
                    //we are either using a bounding rect or a bounding sphere
                    //try to move the object into an enclosing parent node until we've got full containment
                    if (movedObj.EnclosingBox.Max != movedObj.EnclosingBox.Min)
                    {
                        while (current.m_region.Contains(movedObj.EnclosingBox) != ContainmentType.Contains)
                            if (current._parent != null) current = current._parent;
                            else
                            {
                                break; //prevent infinite loops when we go out of bounds of the root node region
                            }
                    }
                    else
                    {
                        ContainmentType ct = current.m_region.Contains(movedObj.EnclosingSphere);
                        while (ct != ContainmentType.Contains)//we must be using a bounding sphere, so check for its containment.
                        {
                            if (current._parent != null)
                            {
                                current = current._parent;
                            }
                            else
                            {
                                //the root region cannot contain the object, so we need to completely rebuild the whole tree.
                                //The rarity of this event is rare enough where we can afford to take all objects out of the existing tree and rebuild the entire thing.
                                List<Physical> tmp = m_root.AllObjects();
                                m_root.UnloadContent();
                                Enqueue(tmp);//add to pending queue

                                
                                return;
                            }

                            ct = current.m_region.Contains(movedObj.EnclosingSphere);
                        }
                    }

                        //now, remove the object from the current node and insert it into the current containing node.
                    m_objects.Remove(movedObj);
                    current.Insert(movedObj);   //this will try to insert the object as deep into the tree as we can go.
                }

                //if (bContained == false)
                //{
                //    List<Physical> objList = AllObjects();
                //    foreach (Physical p in objList)
                //        m_pendingInsertion.Enqueue(p);
                //    m_treeBuilt = false;
                //    UpdateTree();
                //}
                

                //now that all objects have moved and they've been placed into their correct nodes in the octree, we can look for collisions.
                if (IsRoot == true)
                {
                    //This will recursively gather up all collisions and create a list of them.
                    //this is simply a matter of comparing all objects in the current root node with all objects in all child nodes.
                    //note: we can assume that every collision will only be between objects which have moved.
                    //note 2: An explosion can be centered on a point but grow in size over time. In this case, you'll have to override the update method for the explosion.
                    List<IntersectionRecord> irList = GetIntersection(new List<Physical>());

                    foreach (IntersectionRecord ir in irList)
                    {
                        if (ir.PhysicalObject != null)
                            ir.PhysicalObject.HandleIntersection(ir);
                        if (ir.OtherPhysicalObject != null)
                            ir.OtherPhysicalObject.HandleIntersection(ir);
                    }
                }
            }//end if tree built
            else
            {
                if (m_pendingInsertion.Count > 0)
                {
                    ProcessPendingItems();
                    Update(time);   //try this again...
                }
            }
        }

        #endregion

        /// <summary>
        /// Inserts a bunch of items into the oct tree.
        /// </summary>
        /// <param name="ItemList">A list of physical objects to add</param>
        /// <remarks>The OctTree will be rebuilt JIT</remarks>
        public void Enqueue<T>(List<T> ItemList) where T : Physical
        {
            foreach (Physical Item in ItemList)
            {
                if (Item.HasBounds)
                {
                    m_pendingInsertion.Enqueue(Item);
                    m_treeReady = false;
                }
                else
                {
                    throw new Exception("Every object being inserted into the octTree must have a bounding region!");
                }
            }

        }

        public void Enqueue<T>(T Item) where T: Physical
        {
            if (Item.HasBounds) //sanity check
            {
                //are we trying to add at the root node? If so, we can assume that the user doesn't know where in the tree it needs to go.
                if (_parent == null)
                {
                    m_pendingInsertion.Enqueue(Item);

                    m_treeReady = false;    //mark the tree as needing an update
                }
                else
                {
                    //the user is giving us a hint on where in the tree they think the object should go. Let's try to insert as close to the hint as possible.
                    OctTree current = this;

                    //push the object up the tree until we find a region which contains it
                    if (Item.EnclosingBox.Max != Item.EnclosingBox.Min)
                    {
                        while (current.m_region.Contains(Item.EnclosingBox) != ContainmentType.Contains)
                            if (current._parent != null) current = current._parent;
                            else break; //prevent infinite loops when we go out of bounds of the root node region
                    }
                    else
                    {
                        while (current.m_region.Contains(Item.EnclosingSphere) != ContainmentType.Contains)//we must be using a bounding sphere, so check for its containment.
                            if (current._parent != null) current = current._parent;
                            else break;
                    }

                    //push the object down the tree if we can.
                    current.Insert(Item);
                }
            }
            else
            {
                throw new Exception("Every object being inserted into the octTree must have a bounding region!");
            }
        }

        //I don't think I actually use this. The octree removes items on its own when they die.
        public bool Remove<T>(T Item) where T : Physical
        {
            //not recursive
            if (m_allObjects.Contains(Item))
            {
                m_allObjects.Remove(Item);
                return true;
            }
            return false;   
        }

        /// <summary>
        /// A tree has already been created, so we're going to try to insert an item into the tree without rebuilding the whole thing
        /// </summary>
        /// <typeparam name="T">A physical object</typeparam>
        /// <param name="Item">The physical object to insert into the tree</param>
        private bool Insert<T>(T Item) where T : Physical
        {
            /*if the current node is an empty leaf node, just insert and leave it.*/
            //if (m_objects.Count == 0 && m_activeNodes == 0)
            if(AllTreeObjects.Count == 0)
            {
                m_objects.Add(Item);
                return true;
            }

            //Check to see if the dimensions of the box are greater than the minimum dimensions.
            //If we're at the smallest size, just insert the item here. We can't go any lower!
            Vector3 dimensions = m_region.Max - m_region.Min;
            if (dimensions.X <= MIN_SIZE && dimensions.Y <= MIN_SIZE && dimensions.Z <= MIN_SIZE)
            {
                m_objects.Add(Item);
                return true;
            }

            //The object won't fit into the current region, so it won't fit into any child regions.
            //therefore, try to push it up the tree. If we're at the root node, we need to resize the whole tree.
            if (m_region.Contains(Item.EnclosingSphere) != ContainmentType.Contains)
            {
                if (this._parent != null)
                    return this._parent.Insert(Item);
                else
                    return false;
            }
            
            //At this point, we at least know this region can contain the object but there are child nodes. Let's try to see if the object will fit
            //within a subregion of this region.

            Vector3 half = dimensions / 2.0f;
            Vector3 center = m_region.Min + half;

            //Find or create subdivided regions for each octant in the current region
            BoundingBox[] childOctant = new BoundingBox[8];
            childOctant[0] = (m_childNode[0] != null) ? m_childNode[0].m_region : new BoundingBox(m_region.Min, center);
            childOctant[1] = (m_childNode[1] != null) ? m_childNode[1].m_region : new BoundingBox(new Vector3(center.X, m_region.Min.Y, m_region.Min.Z), new Vector3(m_region.Max.X, center.Y, center.Z));
            childOctant[2] = (m_childNode[2] != null) ? m_childNode[2].m_region : new BoundingBox(new Vector3(center.X, m_region.Min.Y, center.Z), new Vector3(m_region.Max.X, center.Y, m_region.Max.Z));
            childOctant[3] = (m_childNode[3] != null) ? m_childNode[3].m_region : new BoundingBox(new Vector3(m_region.Min.X, m_region.Min.Y, center.Z), new Vector3(center.X, center.Y, m_region.Max.Z));
            childOctant[4] = (m_childNode[4] != null) ? m_childNode[4].m_region : new BoundingBox(new Vector3(m_region.Min.X, center.Y, m_region.Min.Z), new Vector3(center.X, m_region.Max.Y, center.Z));
            childOctant[5] = (m_childNode[5] != null) ? m_childNode[5].m_region : new BoundingBox(new Vector3(center.X, center.Y, m_region.Min.Z), new Vector3(m_region.Max.X, m_region.Max.Y, center.Z));
            childOctant[6] = (m_childNode[6] != null) ? m_childNode[6].m_region : new BoundingBox(center, m_region.Max);
            childOctant[7] = (m_childNode[7] != null) ? m_childNode[7].m_region : new BoundingBox(new Vector3(m_region.Min.X, center.Y, center.Z), new Vector3(center.X, m_region.Max.Y, m_region.Max.Z));

            //First, is the item completely contained within the root bounding box?
            //note2: I shouldn't actually have to compensate for this. If an object is out of our predefined bounds, then we have a problem/error.
            //          Wrong. Our initial bounding box for the terrain is constricting its height to the highest peak. Flying units will be above that.
            //             Fix: I resized the enclosing box to 256x256x256. This should be sufficient.
            if (Item.EnclosingBox.Max != Item.EnclosingBox.Min && m_region.Contains(Item.EnclosingBox) == ContainmentType.Contains)
            {
                bool found = false;
                //we will try to place the object into a child node. If we can't fit it in a child node, then we insert it into the current node object list.
                for(int a=0;a<8;a++)
                {
                    //is the object fully contained within a quadrant?
                    if (childOctant[a].Contains(Item.EnclosingBox) == ContainmentType.Contains)
                    {
                        if (m_childNode[a] != null)
                        {
                            return m_childNode[a].Insert(Item);   //Add the item into that tree and let the child tree figure out what to do with it
                        }
                        else
                        {
                            m_childNode[a] = CreateNode(childOctant[a], Item);   //create a new tree node with the item
                            m_activeNodes |= (byte)(1 << a);
                        }
                        found = true;
                    }
                }

                //we couldn't fit the item into a smaller box, so we'll have to insert it in this region
                if (!found)
                {
                    m_objects.Add(Item);
                    return true;
                }
            }
            else if (Item.EnclosingSphere.Radius != 0 && m_region.Contains(Item.EnclosingSphere) == ContainmentType.Contains)
            {
                bool found = false;
                //we will try to place the object into a child node. If we can't fit it in a child node, then we insert it into the current node object list.
                for (int a = 0; a < 8; a++)
                {
                    //is the object contained within a child quadrant?
                    if (childOctant[a].Contains(Item.EnclosingSphere) == ContainmentType.Contains)
                    {
                        if (m_childNode[a] != null)
                        {
                            return m_childNode[a].Insert(Item);   //Add the item into that tree and let the child tree figure out what to do with it
                        }
                        else
                        {
                            m_childNode[a] = CreateNode(childOctant[a], Item);   //create a new tree node with the item
                            m_activeNodes |= (byte)(1 << a);
                        }
                        found = true;
                    }
                }

                //we couldn't fit the item into a smaller box, so we'll have to insert it in this region
                if (!found)
                {
                    m_objects.Add(Item);
                    return true;
                }
            }

            //either the item lies outside of the enclosed bounding box or it is intersecting it. Either way, we need to rebuild
            //the entire tree by enlarging the containing bounding box
            return false;
        }

        /// <summary>
        /// RECURSIVE:
        /// Naively builds an oct tree from scratch
        /// </summary>
        private void BuildTree()    //complete & tested
        {
            //terminate the recursion if we're a leaf node
            if (m_objects.Count <= 1)
            {
                if (m_objects.Count == 1 && m_region.Contains(m_objects[0].EnclosingSphere) == ContainmentType.Contains)
                {
                    return;
                }
            }

            Vector3 dimensions = m_region.Max - m_region.Min;

            if (dimensions == Vector3.Zero)
            {
                BoundingBox tightBox = Calc.FindEnclosingBoundingBox(m_objects);
                m_region = Calc.FindEnclosingCube(tightBox);
                dimensions = m_region.Max - m_region.Min;
            }

            //Check to see if the dimensions of the box are greater than the minimum dimensions
            if (dimensions.X <= MIN_SIZE && dimensions.Y <= MIN_SIZE && dimensions.Z <= MIN_SIZE)
            {
                return;
            }

            Vector3 half = dimensions / 2.0f;
            Vector3 center = m_region.Min + half;

            //Create subdivided regions for each octant
            BoundingBox[] octant = new BoundingBox[8];
            octant[0] = new BoundingBox(m_region.Min, center);
            octant[1] = new BoundingBox(new Vector3(center.X, m_region.Min.Y, m_region.Min.Z), new Vector3(m_region.Max.X, center.Y, center.Z));
            octant[2] = new BoundingBox(new Vector3(center.X, m_region.Min.Y, center.Z), new Vector3(m_region.Max.X, center.Y, m_region.Max.Z));
            octant[3] = new BoundingBox(new Vector3(m_region.Min.X, m_region.Min.Y, center.Z), new Vector3(center.X, center.Y, m_region.Max.Z));
            octant[4] = new BoundingBox(new Vector3(m_region.Min.X, center.Y, m_region.Min.Z), new Vector3(center.X, m_region.Max.Y, center.Z));
            octant[5] = new BoundingBox(new Vector3(center.X, center.Y, m_region.Min.Z), new Vector3(m_region.Max.X, m_region.Max.Y, center.Z));
            octant[6] = new BoundingBox(center, m_region.Max);
            octant[7] = new BoundingBox(new Vector3(m_region.Min.X, center.Y, center.Z), new Vector3(center.X, m_region.Max.Y, m_region.Max.Z));

            //This will contain all of our objects which fit within each respective octant.
            List<Physical>[] octList = new List<Physical>[8];
            for (int i = 0; i < 8; i++) octList[i] = new List<Physical>();

            //this list contains all of the objects which got moved down the tree and can be delisted from this node.
            List<Physical> delist = new List<Physical>();

            foreach (Physical obj in m_objects)
            {
                if (obj.EnclosingBox.Min != obj.EnclosingBox.Max)
                {
                    for (int a = 0; a < 8; a++)
                    {
                        if (octant[a].Contains(obj.EnclosingBox) == ContainmentType.Contains)
                        {
                            octList[a].Add(obj);
                            delist.Add(obj);
                            break;
                        }
                    }
                }
                else if (obj.EnclosingSphere.Radius != 0)
                {
                    for (int a = 0; a < 8; a++)
                    {
                        if (octant[a].Contains(obj.EnclosingSphere) == ContainmentType.Contains)
                        {
                            octList[a].Add(obj);
                            delist.Add(obj);
                            break;
                        }
                    }
                }
            }

            //delist every moved object from this node.
            foreach (Physical obj in delist)
                m_objects.Remove(obj);

            if (m_childNode == null)
                m_childNode = new OctTree[8];
            //m_activeNodes = 0;

            //Create child nodes where there are items contained in the bounding region
            for (int a = 0; a < 8; a++)
            {
                if (octList[a].Count != 0)
                {
                    m_childNode[a] = CreateNode(octant[a], octList[a]);
                    m_activeNodes |= (byte)(1 << a);
                    m_childNode[a].BuildTree();
                }
            }

            //m_treeBuilt = true;
            //m_treeReady = true;
        }

        private void SimpleTreeInit()
        {
            //clean out any exist tree data
            //m_root = new OctTree();
            m_root = this;

            UnloadContent();

            BoundingBox tightBox = Calc.FindEnclosingBoundingBox(m_allObjects);
            m_region = Calc.FindEnclosingCube(tightBox);

            m_objects.AddRange(m_allObjects);

            SimpleTreeBuildHelper(m_root);

            m_treeReady = true;
            m_treeBuilt = true;
        }

        private void SimpleTreeBuildHelper(OctTree currentNode)
        {
            //since this node only has one object, we don't have to subdivide it anymore.
            if (currentNode.m_objects.Count == 1)
                return;

            BoundingBox[] octants = GetSubdividedOctants(currentNode);

            //This will contain all of our objects which fit within each respective octant.
            List<Physical>[] octList = new List<Physical>[8];
            for (int i = 0; i < 8; i++) octList[i] = new List<Physical>();

            //this list contains all of the objects which got moved down the tree and can be delisted from this node.
            List<Physical> delist = new List<Physical>();

            foreach (Physical obj in currentNode.m_objects)
            {
                int index = FindContainingRegion(obj, octants);

                if (index >= 0)
                {
                    octList[index].Add(obj);
                    delist.Add(obj);
                }
            }

            //delist every placed object from this nodes list of objects.
            foreach (Physical obj in delist)
                currentNode.m_objects.Remove(obj);

            if (currentNode.m_childNode == null)
                currentNode.m_childNode = new OctTree[8];

            for (int a = 0; a < 8; a++)
            {
                if (octList[a].Count != 0)
                {
                    currentNode.m_childNode[a] = CreateNode(octants[a], octList[a]);
                    currentNode.m_activeNodes |= (byte)(1 << a);
                    currentNode.SimpleTreeBuildHelper(currentNode.m_childNode[a]);
                }
            }
        }

        public void SimpleTreeUpdate(coreTime worldTime)
        {

            if (IsEmpty) return;

            //BaseSettings.StartPerf("Tree Build");       //~250fps
            //rebuild the whole tree
            if (m_treeReady == false)
            {
                SimpleTreeInit();
            }
            //BaseSettings.StopPerf("Tree Build");

            //update all objects in the collection
            //BaseSettings.StartPerf("Tree Object Update");       //~30fps
            BaseSettings.StartPerfColl("Tree Object Update", m_allObjects.Count);
            int count = m_allObjects.Count;
            for (int a = 0; a < count;a++ )
            {
                BaseSettings.StartPerfCollItem("Tree Object Update", a);
                int status = m_allObjects[a].Update(worldTime);
                BaseSettings.StopPerfCollItem("Tree Object Update", a);

                if(status != 0)
                {
                    m_treeReady = false;
                }

                if (status == 2)
                {
                    m_allObjects.Remove(m_allObjects[a--]);
                    count--;
                    continue;
                }

            }
            BaseSettings.StopPerfColl("Tree Object Update");
            //BaseSettings.StopPerf("Tree Object Update");

            
        }

        public void SimpleTreeAdd<T>(T Item) where T : Physical
        {
            m_treeReady = false;
            m_allObjects.Add(Item);
        }

        public void SimpleTreeAdd<T>(List<T> ItemList) where T : Physical
        {
            m_treeReady = false;
            m_allObjects.AddRange(ItemList);
        }

        public void SimpleTreeRemove<T>(T Item) where T : Physical
        {
            m_treeReady = false;
            m_allObjects.Remove(Item);
        }

        /// <summary>
        /// This will try to find where to place the item within a bounding region.
        /// </summary>
        /// <param name="item">The item to be placed</param>
        /// <param name="regions">An array of 8 bounding boxes to try to place the item into</param>
        /// <returns>
        /// -2: Input parameters are null!
        /// -1: The object could not be placed
        /// 0-7: The index of the bounding box which the item can be contained within</returns>
        private int FindContainingRegion(Physical Item, BoundingBox[] regions)
        {
            if (Item == null) return -2;
            if (regions == null) return -2;

            if (Item.EnclosingBox.Max != Item.EnclosingBox.Min)
            {
                //we will try to place the object into a child node. If we can't fit it in a child node, then we insert it into the current node object list.
                for (int a = 0; a < 8; a++)
                {
                    //is the object fully contained within a quadrant?
                    if (regions[a].Contains(Item.EnclosingBox) == ContainmentType.Contains)
                    {
                        return a;
                    }
                }
            }
            else if (Item.EnclosingSphere.Radius != 0)
            {
                //we will try to place the object into a child node. If we can't fit it in a child node, then we insert it into the current node object list.
                for (int a = 0; a < 8; a++)
                {
                    //is the object contained within a child quadrant?
                    if (regions[a].Contains(Item.EnclosingSphere) == ContainmentType.Contains)
                    {
                        return a;
                    }
                }
            }

            return -1;
        }

        /// <summary>
        /// Calculates the sub octant regions for the given region
        /// </summary>
        /// <param name="region">A non-zero sized region of space</param>
        /// <returns>An array containing 8 bounding boxes which subdivide the given region</returns>
        private BoundingBox[] GetSubdividedOctants(BoundingBox region)
        {
            Vector3 dimensions = region.Max - region.Min;

            if (dimensions == Vector3.Zero)
            {
                //return null;
                throw new Exception("zero dimension regions cant be split!");
            }

            Vector3 half = dimensions / 2.0f;
            Vector3 center = region.Min + half;

            //Create subdivided regions for each octant
            BoundingBox[] octant = new BoundingBox[8];
            octant[0] = new BoundingBox(region.Min, center);
            octant[1] = new BoundingBox(new Vector3(center.X, region.Min.Y, region.Min.Z), new Vector3(region.Max.X, center.Y, center.Z));
            octant[2] = new BoundingBox(new Vector3(center.X, region.Min.Y, center.Z), new Vector3(region.Max.X, center.Y, region.Max.Z));
            octant[3] = new BoundingBox(new Vector3(region.Min.X, region.Min.Y, center.Z), new Vector3(center.X, center.Y, region.Max.Z));
            octant[4] = new BoundingBox(new Vector3(region.Min.X, center.Y, region.Min.Z), new Vector3(center.X, region.Max.Y, center.Z));
            octant[5] = new BoundingBox(new Vector3(center.X, center.Y, region.Min.Z), new Vector3(region.Max.X, region.Max.Y, center.Z));
            octant[6] = new BoundingBox(center, region.Max);
            octant[7] = new BoundingBox(new Vector3(region.Min.X, center.Y, center.Z), new Vector3(center.X, region.Max.Y, region.Max.Z));

            return octant;
        }

        /// <summary>
        /// Finds/Creates a list of subdivided octants for the given node. If the node already contains a few subdivided octants,
        /// it will use those existing regions.
        /// </summary>
        /// <param name="currentNode">The current node to find subdivided regions for</param>
        /// <returns>An 8 element array of bounding boxes indicating the subdivided regions</returns>
        private BoundingBox[] GetSubdividedOctants(OctTree currentNode)
        {
            Vector3 dimensions = currentNode.m_region.Max - currentNode.m_region.Min;

            if (dimensions == Vector3.Zero)
            {
                //return null;
                throw new Exception("zero dimension regions cant be split!");
            }

            Vector3 half = dimensions / 2.0f;
            Vector3 center = currentNode.m_region.Min + half;

            BoundingBox[] childOctant = new BoundingBox[8];
            childOctant[0] = (currentNode.m_childNode[0] != null) ? currentNode.m_childNode[0].m_region : new BoundingBox(currentNode.m_region.Min, center);
            childOctant[1] = (currentNode.m_childNode[1] != null) ? currentNode.m_childNode[1].m_region : new BoundingBox(new Vector3(center.X, currentNode.m_region.Min.Y, currentNode.m_region.Min.Z), new Vector3(currentNode.m_region.Max.X, center.Y, center.Z));
            childOctant[2] = (currentNode.m_childNode[2] != null) ? currentNode.m_childNode[2].m_region : new BoundingBox(new Vector3(center.X, currentNode.m_region.Min.Y, center.Z), new Vector3(currentNode.m_region.Max.X, center.Y, currentNode.m_region.Max.Z));
            childOctant[3] = (currentNode.m_childNode[3] != null) ? currentNode.m_childNode[3].m_region : new BoundingBox(new Vector3(currentNode.m_region.Min.X, currentNode.m_region.Min.Y, center.Z), new Vector3(center.X, center.Y, currentNode.m_region.Max.Z));
            childOctant[4] = (currentNode.m_childNode[4] != null) ? currentNode.m_childNode[4].m_region : new BoundingBox(new Vector3(currentNode.m_region.Min.X, center.Y, currentNode.m_region.Min.Z), new Vector3(center.X, currentNode.m_region.Max.Y, center.Z));
            childOctant[5] = (currentNode.m_childNode[5] != null) ? currentNode.m_childNode[5].m_region : new BoundingBox(new Vector3(center.X, center.Y, currentNode.m_region.Min.Z), new Vector3(currentNode.m_region.Max.X, currentNode.m_region.Max.Y, center.Z));
            childOctant[6] = (currentNode.m_childNode[6] != null) ? currentNode.m_childNode[6].m_region : new BoundingBox(center, currentNode.m_region.Max);
            childOctant[7] = (currentNode.m_childNode[7] != null) ? currentNode.m_childNode[7].m_region : new BoundingBox(new Vector3(currentNode.m_region.Min.X, center.Y, center.Z), new Vector3(center.X, currentNode.m_region.Max.Y, currentNode.m_region.Max.Z));

            return childOctant;
        }

        private OctTree CreateNode(BoundingBox region, List<Physical> objList)  //complete & tested
        {
            if (objList.Count == 0)
                return null;

            OctTree ret = new OctTree(region, objList);
            ret._parent = this;

            return ret;
        }

        private OctTree CreateNode(BoundingBox region, Physical Item)
        {
            List<Physical> objList = new List<Physical>(1); //sacrifice potential CPU time for a smaller memory footprint
            objList.Add(Item);
            OctTree ret = new OctTree(region, objList);
            ret._parent = this;
            return ret;
        }

        /// <summary>
        /// This grabs the current container being occupied by the object.
        /// </summary>
        /// <param name="myObject">The object you're looking for</param>
        /// <param name="searchNode">The node you want to start searching in</param>
        /// <returns>Null: no container was found
        /// Octree: the container which currently holds the object</returns>
        private OctTree FindObjectInTree(Physical myObject, OctTree currentNode)
        {
            if (currentNode.m_objects.Contains(myObject))
            {
                return currentNode;
            }
            else
            {
                //Create subdivided regions for each octant
                BoundingBox[] octant = GetSubdividedOctants(currentNode.m_region);


                if (myObject.EnclosingBox.Min != myObject.EnclosingBox.Max)
                {
                    for (int a = 0; a < 8; a++)
                    {
                        if (octant[a].Contains(myObject.EnclosingBox) == ContainmentType.Contains)
                        {
                            if (currentNode.m_childNode[a] != null)
                                return FindObjectInTree(myObject, currentNode.m_childNode[a]);
                            else
                                break;
                        }
                    }
                }
                else if (myObject.EnclosingSphere.Radius != 0)
                {
                    for (int a = 0; a < 8; a++)
                    {
                        if (octant[a].Contains(myObject.EnclosingSphere) == ContainmentType.Contains)
                        {
                            if (currentNode.m_childNode[a] != null)
                                return FindObjectInTree(myObject, currentNode.m_childNode[a]);
                            else
                                break;
                        }
                    }
                }

                //we couldn't fit the object into any child nodes and its not in the current node. There's only one last possibility:
                //the object is in a child node but has moved such that it intersects two bounding boxes. In that case, one of the child
                //nodes may still contain the object.
                return FindObjectByBrute(myObject, currentNode);
            }
        }

        private OctTree FindObjectByBrute(Physical myObject, OctTree currentNode)
        {
            if (currentNode.m_objects.Contains(myObject))
                return currentNode;
            else
            {
                if (currentNode.HasChildren)
                {
                    for (int a = 0; a < 8; a++)
                    {
                        if (currentNode.m_childNode[a] != null)
                        {
                            OctTree result = FindObjectByBrute(myObject, currentNode.m_childNode[a]);
                            if (result != null)
                                return result;
                        }
                    }
                }
                return null;
            }
        }

        private OctTree FindContainingChildnode(Physical myObject, OctTree currentNode)
        {
            //if (m_objects.Contains(myObject))
            //{
            //    return this;
            //}


            /*We aren't looking for the object within a node, we're just looking for the node which should contain the object.*/

            BoundingBox[] octant = GetSubdividedOctants(currentNode.m_region);


            if (myObject.EnclosingBox.Min != myObject.EnclosingBox.Max)
            {
                for (int a = 0; a < 8; a++)
                {
                    if (octant[a].Contains(myObject.EnclosingBox) == ContainmentType.Contains)
                    {
                        if (currentNode.m_childNode[a] != null)
                            return FindContainingChildnode(myObject, currentNode.m_childNode[a]);
                        else
                            break;
                    }
                }
            }
            else if (myObject.EnclosingSphere.Radius != 0)
            {
                for (int a = 0; a < 8; a++)
                {
                    if (octant[a].Contains(myObject.EnclosingSphere) == ContainmentType.Contains)
                    {
                        if (currentNode.m_childNode[a] != null)
                            return FindContainingChildnode(myObject, currentNode.m_childNode[a]);
                        else
                            break;
                    }
                }
            }

            //we couldn't fit the object into any child nodes and its not in the current node. There's only one last possibility:
            //the object is in a child node but has moved such that it intersects two bounding boxes. In that case, one of the child
            //nodes may still contain the object.

            return currentNode;
        }

        public void ExportXML()
        {
            CommandFileWriter writer = new CommandFileWriter("Content\\XML\\Octree.xml");
            writer.PushNode("Octree");
            XMLWrite(this, writer);
            writer.PopNode();
            writer.Close();
        }

        private void XMLWrite(OctTree currentNode, CommandFileWriter writer)
        {
            if (currentNode._parent == null)
            {
                writer.PushNode("Root");
            }
            else if (currentNode.HasChildren)
            {
                writer.PushNode("Branch");
            }
            else
            {
                writer.PushNode("Leaf");
            }
            writer.AddAttribute("Life", currentNode.m_curLife);
            if(currentNode.m_activeNodes != 0)
                writer.AddAttribute("Active_Nodes", currentNode.m_activeNodes);
            string children = "";
            for (int flags = currentNode.m_activeNodes, index = 0; flags > 0; flags >>= 1, index++)
            {
                if ((flags & 1) == 1)                                           //is this an active child node?
                    children += "[" + currentNode.m_childNode[index].AllTreeObjects.Count + "]";
                else
                    children += "[0]";
            }
            if (children != "")
            {
                writer.AddAttribute("Child_Nodes", children);
            }
            writer.AddAttribute("Nested_Objects", currentNode.AllTreeObjects.Count);

            if (currentNode.m_objects.Count > 0)
            {
                
                writer.PushNode("Objects");
                writer.AddAttribute("Total", currentNode.m_objects.Count);
                foreach (Physical obj in currentNode.m_objects)
                {
                    writer.PushNode("Object");
                    writer.AddValue(obj.ToString());
                    writer.PopNode();
                }
                writer.PopNode();
            }

            for (int flags = currentNode.m_activeNodes, index = 0; flags > 0; flags >>= 1, index++)
            {
                if ((flags & 1) == 1)                                           //is this an active child node?
                    XMLWrite(currentNode.m_childNode[index], writer);
            }

            writer.PopNode();
        }

        private void PruneDeadBranches(OctTree currentNode)
        {
            //Start a count down death timer for any leaf nodes which don't have objects or children.
            //when the timer reaches zero, we delete the leaf. If the node is reused before death, we double its lifespan.
            //this gives us a "frequency" usage score and lets us avoid allocating and deallocating memory unnecessarily

            if (currentNode == null)
                ExportXML();

            if (currentNode.m_objects.Count == 0)           //node is empty
            {
                if (currentNode.HasChildren == false)       //node is a leaf node with no objects
                {
                    if (currentNode.m_curLife == -1)        //node countdown timer is inactive
                        currentNode.m_curLife = currentNode.m_maxLifespan;
                    else if (currentNode.m_curLife > 0)                 //node countdown time is active
                    {
                        currentNode.m_curLife--;
                    }
                }
            }
            else
            {
                if (currentNode.m_curLife != -1)            //node countdown timer is active and it now has objects!
                {
                    if (currentNode.m_maxLifespan <= 64)    //double the max life of the timer and reset the timer
                        currentNode.m_maxLifespan *= 2;
                    currentNode.m_curLife = -1;
                }
            }

            //prune out any dead branches in the tree
            for (int flags = currentNode.m_activeNodes, index = 0; flags > 0; flags >>= 1, index++)
            {
                if ((flags & 1) == 1)                                           //is this an active child node?
                {
                    PruneDeadBranches(currentNode.m_childNode[index]);          //try to recursively prune any dead child nodes

                    if (currentNode.m_childNode[index].m_curLife == 0)          //has the death timer completed?
                    {
                        if (currentNode.m_childNode[index].m_objects.Count > 0)
                        {
                            /*If this happens, an object moved into our node and we didn't catch it. That means we have to do a conceptual rethink on this implementation.*/
                            throw new Exception("Tried to delete a used branch!");
                            //currentNode.m_childNode[index].m_curLife = -1;
                        }
                        else
                        {
                            currentNode.m_childNode[index] = null;
                            currentNode.m_activeNodes ^= (byte)(1 << index);       //remove the node from the active nodes flag list
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Recursive: 
        /// Updates all objects in the given tree and gives you a list of objects which moved during their update.
        /// Will also prune out any dead objects.
        /// </summary>
        /// <param name="currentNode">The node you're updating</param>
        /// <param name="gameTime"></param>
        /// <returns>A list of moved objects</returns>
        private void UpdateTreeObjects(OctTree currentNode, coreTime time)
        {
            //go through and update every object in the node

            m_deadObjects.Clear();
            List<Physical> movedObjects = new List<Physical>();
            
            //Update & move all objects in the node
            for (int a = 0; a < currentNode.m_objects.Count; a++)
            {
                //we should figure out if an object actually moved so that we know whether we need to update this node in the tree.
                if (currentNode.m_objects[a].Update(time) == 1)
                    movedObjects.Add(currentNode.m_objects[a]);

                if (!currentNode.m_objects[a].Alive)
                    m_deadObjects.Add(currentNode.m_objects[a]);
            }

            //prune any dead objects from the tree.
            foreach (Physical deadObj in m_deadObjects)
            {
                //maintain the moved objects list
                if (movedObjects.Contains(deadObj))
                    movedObjects.Remove(deadObj);

                //remove the object from the octree
                currentNode.m_objects.Remove(deadObj);

                //remove the object from the global objects list
                m_allObjects.Remove(deadObj);
            }

            //update any child nodes
            if (currentNode.HasChildren)
            {
                for (int flags = currentNode.m_activeNodes, index = 0; flags > 0; flags >>= 1, index++)
                {
                    if ((flags & 1) == 1)                                           //is this an active child node?
                    {
                        UpdateTreeObjects(currentNode.m_childNode[index], time);
                    }
                }
            }

            //If an object moved, we can insert it into the parent and that will insert it into the correct tree node.
            //note that we have to do this last so that we don't accidentally update the same object more than once per frame.
            foreach (Physical movedObj in movedObjects)
            {
                OctTree current = currentNode;
                currentNode.Remove(movedObj);

                //figure out how far up the tree we need to go to reinsert our moved object
                //we are either using a bounding rect or a bounding sphere
                //try to move the object into an enclosing parent node until we've got full containment
                if (movedObj.EnclosingBox.Max != movedObj.EnclosingBox.Min)
                {
                    while (current.m_region.Contains(movedObj.EnclosingBox) != ContainmentType.Contains)
                        if (current._parent != null) current = current._parent;
                        else
                        {
                            break; //prevent infinite loops when we go out of bounds of the root node region
                        }
                }
                else
                {
                    ContainmentType ct = current.m_region.Contains(movedObj.EnclosingSphere);
                    while (ct != ContainmentType.Contains)//we must be using a bounding sphere, so check for its containment.
                    {
                        if (current._parent != null)
                        {
                            current = current._parent;
                        }
                        else
                        {
                            //the root region cannot contain the object, so we need to completely rebuild the whole tree.
                            //The rarity of this event is rare enough where we can afford to take all objects out of the existing tree and rebuild the entire thing.
                            List<Physical> tmp = m_root.AllObjects();
                            m_root.UnloadContent();
                            Enqueue(tmp);//add to pending queue


                            return;
                        }

                        ct = current.m_region.Contains(movedObj.EnclosingSphere);
                    }
                }

                //now, remove the object from the current node and insert it into the current containing node.
                //m_objects.Remove(movedObj);
                current.Insert(movedObj);   //this will try to insert the object as deep into the tree as we can go.
            }

            ////all objects which have moved need to be tested to see if they still belong in the current node
            //foreach (Physical movedObj in movedObjects)
            //{
            //    
                
            //    UpdateMovedObjectTreePosition(currentNode, movedObj);
            //}
        }

        private bool UpdateMovedObjectTreePosition(OctTree currentNode, Physical Item)
        {
            /*if the current node is an empty leaf node, just insert and leave it.*/
            //if (m_objects.Count == 0 && m_activeNodes == 0)
            if (currentNode.AllTreeObjects.Count == 0)
            {
                currentNode.m_objects.Add(Item);
                return true;
            }

            //Check to see if the dimensions of the box are greater than the minimum dimensions.
            //If we're at the smallest size, just insert the item here. We can't go any lower!
            Vector3 dimensions = currentNode.m_region.Max - currentNode.m_region.Min;
            if (dimensions.X <= MIN_SIZE && dimensions.Y <= MIN_SIZE && dimensions.Z <= MIN_SIZE)
            {
                currentNode.m_objects.Add(Item);
                return true;
            }

            //The object won't fit into the current region, so it won't fit into any child regions.
            //therefore, try to push it up the tree. If we're at the root node, we need to resize the whole tree.
            if (currentNode.m_region.Contains(Item.EnclosingSphere) != ContainmentType.Contains)
            {
                if (currentNode._parent != null)
                    currentNode.UpdateMovedObjectTreePosition(_parent, Item);
                else
                {
                    //gotta rebuild the whole tree
                    List<Physical> tmp = m_root.AllObjects();
                    UnloadContent();
                    m_root = new OctTree(new BoundingBox(Vector3.Zero, Vector3.Zero));
                    Enqueue(tmp);
                    ProcessPendingItems();

                    return false;
                }
            }

            Vector3 half = dimensions / 2.0f;
            Vector3 center = m_region.Min + half;

            //Find or create subdivided regions for each octant in the current region
            BoundingBox[] childOctant = new BoundingBox[8];
            childOctant[0] = (currentNode.m_childNode[0] != null) ? currentNode.m_childNode[0].m_region : new BoundingBox(currentNode.m_region.Min, center);
            childOctant[1] = (currentNode.m_childNode[1] != null) ? currentNode.m_childNode[1].m_region : new BoundingBox(new Vector3(center.X, currentNode.m_region.Min.Y, currentNode.m_region.Min.Z), new Vector3(currentNode.m_region.Max.X, center.Y, center.Z));
            childOctant[2] = (currentNode.m_childNode[2] != null) ? currentNode.m_childNode[2].m_region : new BoundingBox(new Vector3(center.X, currentNode.m_region.Min.Y, center.Z), new Vector3(currentNode.m_region.Max.X, center.Y, currentNode.m_region.Max.Z));
            childOctant[3] = (currentNode.m_childNode[3] != null) ? currentNode.m_childNode[3].m_region : new BoundingBox(new Vector3(currentNode.m_region.Min.X, currentNode.m_region.Min.Y, center.Z), new Vector3(center.X, center.Y, currentNode.m_region.Max.Z));
            childOctant[4] = (currentNode.m_childNode[4] != null) ? currentNode.m_childNode[4].m_region : new BoundingBox(new Vector3(currentNode.m_region.Min.X, center.Y, currentNode.m_region.Min.Z), new Vector3(center.X, currentNode.m_region.Max.Y, center.Z));
            childOctant[5] = (currentNode.m_childNode[5] != null) ? currentNode.m_childNode[5].m_region : new BoundingBox(new Vector3(center.X, center.Y, currentNode.m_region.Min.Z), new Vector3(currentNode.m_region.Max.X, currentNode.m_region.Max.Y, center.Z));
            childOctant[6] = (currentNode.m_childNode[6] != null) ? currentNode.m_childNode[6].m_region : new BoundingBox(center, currentNode.m_region.Max);
            childOctant[7] = (currentNode.m_childNode[7] != null) ? currentNode.m_childNode[7].m_region : new BoundingBox(new Vector3(currentNode.m_region.Min.X, center.Y, center.Z), new Vector3(center.X, currentNode.m_region.Max.Y, currentNode.m_region.Max.Z));

            //First, is the item completely contained within the root bounding box?
            //note2: I shouldn't actually have to compensate for this. If an object is out of our predefined bounds, then we have a problem/error.
            //          Wrong. Our initial bounding box for the terrain is constricting its height to the highest peak. Flying units will be above that.
            //             Fix: I resized the enclosing box to 256x256x256. This should be sufficient.
            if (Item.EnclosingBox.Max != Item.EnclosingBox.Min && currentNode.m_region.Contains(Item.EnclosingBox) == ContainmentType.Contains)
            {
                bool found = false;
                //we will try to place the object into a child node. If we can't fit it in a child node, then we insert it into the current node object list.
                for (int a = 0; a < 8; a++)
                {
                    //is the object fully contained within a quadrant?
                    if (childOctant[a].Contains(Item.EnclosingBox) == ContainmentType.Contains)
                    {
                        if (currentNode.m_childNode[a] != null)
                        {
                            currentNode.m_childNode[a].UpdateMovedObjectTreePosition(currentNode.m_childNode[a], Item);   //Add the item into that tree and let the child tree figure out what to do with it
                            break;
                        }
                        else
                        {
                            currentNode.m_childNode[a] = CreateNode(childOctant[a], Item);   //create a new tree node with the item
                            currentNode.m_activeNodes |= (byte)(1 << a);
                        }
                        found = true;
                    }
                }

                //we couldn't fit the item into a smaller box, so we'll have to insert it in this region
                if (!found)
                {
                    currentNode.m_objects.Add(Item);
                }
            }
            else if (Item.EnclosingSphere.Radius != 0 && currentNode.m_region.Contains(Item.EnclosingSphere) == ContainmentType.Contains)
            {
                bool found = false;
                //we will try to place the object into a child node. If we can't fit it in a child node, then we insert it into the current node object list.
                for (int a = 0; a < 8; a++)
                {
                    //is the object contained within a child quadrant?
                    if (childOctant[a].Contains(Item.EnclosingSphere) == ContainmentType.Contains)
                    {
                        if (currentNode.m_childNode[a] != null)
                        {
                            currentNode.m_childNode[a].UpdateMovedObjectTreePosition(currentNode.m_childNode[a], Item);   //Add the item into that tree and let the child tree figure out what to do with it
                            break;
                        }
                        else
                        {
                            currentNode.m_childNode[a] = CreateNode(childOctant[a], Item);   //create a new tree node with the item
                            currentNode.m_activeNodes |= (byte)(1 << a);
                        }
                        found = true;
                    }
                }

                //we couldn't fit the item into a smaller box, so we'll have to insert it in this region
                if (!found)
                {
                    currentNode.m_objects.Add(Item);
                }
            }

            return true;
        }

        /// <summary>
        /// Processes all pending insertions by inserting them into the tree.
        /// </summary>
        /// <remarks>Consider deprecating this?</remarks>
        private void ProcessPendingItems()   //complete & tested
        {
            if (this._parent == null)
                m_root = this;

            if (m_objects == null)
                m_objects = new List<Physical>();

            m_allObjects.Clear();
            m_allObjects.AddRange(m_pendingInsertion);

            /*I think I can just directly insert items into the tree instead of using a queue.*/
            if (!m_treeBuilt)
            {
                m_objects.AddRange(m_pendingInsertion);
                m_pendingInsertion.Clear();

                //trim out any objects which have the exact same bounding areas

                BuildTree();

                m_treeBuilt = true;
                m_treeReady = true;     //we know that since no tree existed, this is the first time we're ever here.
            }
            else
            {
                //A tree structure exists already, so we just want to try to insert into the existing structure.
                //bug test: what if the pending item doesn't fit into the bounding region of the existing tree?
                while (m_pendingInsertion.Count != 0)
                {
                    Insert(m_pendingInsertion.Dequeue());
                }
            }
        }
        
        #region Colliders

        /// <summary>
        /// Gives you a list of all intersection records which intersect or are contained within the given frustum area
        /// </summary>
        /// <param name="frustum">The containing frustum to check for intersection/containment with</param>
        /// <returns>A list of intersection records with collisions</returns>
        private List<IntersectionRecord> GetIntersection(BoundingFrustum frustum, PhysicalType type = PhysicalType.ALL)
        {
            if (!m_treeBuilt) return new List<IntersectionRecord>();

            if (m_objects.Count == 0 && HasChildren == false)   //terminator for any recursion
                return null;

            List<IntersectionRecord> ret = new List<IntersectionRecord>();

            //test each object in the list for intersection
            foreach (Physical obj in m_objects)
            {

                //skip any objects which don't meet our type criteria
                if ((int)((int)type & (int)obj.Type) == 0)
                    continue;

                //test for intersection
                IntersectionRecord ir = obj.Intersects(frustum);
                if (ir != null) 
                    ret.Add(ir);
            }

            //test each object in the list for intersection
            for (int a = 0; a < 8; a++)
            {
                if (m_childNode[a] != null && (frustum.Contains(m_childNode[a].m_region) == ContainmentType.Intersects || frustum.Contains(m_childNode[a].m_region) == ContainmentType.Contains))
                {
                    List<IntersectionRecord> hitList = m_childNode[a].GetIntersection(frustum, type);
                    if (hitList != null) ret.AddRange(hitList);
                }
            }
            return ret;
        }

        /// <summary>
        /// Recursively tries to intersect a bounding sphere against all other objects in the octree with the given type
        /// </summary>
        /// <param name="sphere">The bounding volume to intersect all other objects against</param>
        /// <param name="type">The filter for the type of objects which should be tested for hit results</param>
        /// <returns>A list of intersection records which contain the intersection information</returns>
        private List<IntersectionRecord> GetIntersection(BoundingSphere sphere, PhysicalType type = PhysicalType.ALL)
        {
            if (m_objects.Count == 0 && HasChildren == false)   //terminator for any recursion
                return null;

            List<IntersectionRecord> ret = new List<IntersectionRecord>();

            //test each object in the list for intersection
            foreach (Physical obj in m_objects)
            {

                //skip any objects which don't meet our type criteria
                if ((int)((int)type & (int)obj.Type) == 0)
                    continue;

                //test for intersection
                IntersectionRecord ir = obj.Intersects(sphere);
                if (ir != null) 
                    ret.Add(ir);
            }

            //test each object in the list for intersection
            for (int a = 0; a < 8; a++)
            {
                if (m_childNode[a] != null && (sphere.Contains(m_childNode[a].m_region) == ContainmentType.Intersects || sphere.Contains(m_childNode[a].m_region) == ContainmentType.Contains))
                {
                    List<IntersectionRecord> hitList = m_childNode[a].GetIntersection(sphere, type);
                    if (hitList != null)
                    {
                        foreach (IntersectionRecord ir in hitList)
                            ret.Add(ir);
                    }
                }
            }
            return ret;
        }

        /// <summary>
        /// Gives you a list of intersection records for all objects which intersect with the given ray
        /// </summary>
        /// <param name="intersectRay">The ray to intersect objects against</param>
        /// <returns>A list of all intersections</returns>
        private List<IntersectionRecord> GetIntersection(Ray intersectRay, PhysicalType type = PhysicalType.ALL)
        {
            if (!m_treeBuilt) return new List<IntersectionRecord>();

            if (m_objects.Count == 0 && HasChildren == false)   //terminator for any recursion
                return null;

            List<IntersectionRecord> ret = new List<IntersectionRecord>();

            //the ray is intersecting this region, so we have to check for intersection with all of our contained objects and child regions.
            
            //test each object in the list for intersection
            foreach (Physical obj in m_objects)
            {
                //skip any objects which don't meet our type criteria
                if ((int)((int)type & (int)obj.Type) == 0)
                    continue;

                IntersectionRecord ir = obj.Intersects(intersectRay);
                if (ir != null)
                    ret.Add(ir);

                //if (obj.BoundingBox.Max != obj.BoundingBox.Min) //we actually have a legit bounding box
                //{
                //    if (obj.BoundingBox.Intersects(intersectRay) != null)
                //    {
                //        m_lineColor = Color.Red;
                        
                //        ir.PhysicalObject = obj;

                //        if (ir.HasHit)
                //            ret.Add(ir);
                //    }
                //}
                
                //if (obj.BoundingSphere.Radius != 0)    //we actually have a legit bounding sphere
                //{
                //    float? testHit = obj.BoundingSphere.Intersects(intersectRay);
                //    //what about bounding spheres?!
                //    if (testHit != null)
                //    {
                //        //we actually know we have an intersection here, so why test for it again?
                //        IntersectionRecord ir = obj.Intersects(intersectRay);
                //        ir.PhysicalObject = obj;
                //        if (ir.HasHit)
                //            ret.Add(ir);
                //    }
                //}
            }

            // test each child octant for intersection
            for (int a = 0; a < 8; a++)
            {
                if (m_childNode[a] != null && m_childNode[a].m_region.Intersects(intersectRay) != null)
                {
                    m_lineColor = Color.Red;
                    List<IntersectionRecord> hits = m_childNode[a].GetIntersection(intersectRay, type);
                    if (hits != null && hits.Count > 0)
                    {
                        ret.AddRange(hits);
                    }
                }
            }

            return ret;
        }

        private List<IntersectionRecord> GetIntersection(List<Physical> parentObjs, PhysicalType type = PhysicalType.ALL)
        {
            List<IntersectionRecord> intersections = new List<IntersectionRecord>();
            //assume all parent objects have already been processed for collisions against each other.
            //check all parent objects against all objects in our local node
            foreach (Physical pObj in parentObjs)
            {
                foreach (Physical lObj in m_objects)
                {
                    //We let the two objects check for collision against each other. They can figure out how to do the coarse and granular checks.
                    //all we're concerned about is whether or not a collision actually happened.
                    IntersectionRecord ir = pObj.Intersects(lObj);
                    if (ir != null)
                    {

                        //ir.m_treeNode = this;


                        intersections.Add(ir);
                    }
                }
            }

            //now, check all our local objects against all other local objects in the node
            if (m_objects != null && m_objects.Count > 1)
            {
                #region self-congratulation
                /*
                 * This is a rather brilliant section of code. Normally, you'd just have two foreach loops, like so:
                 * foreach(Physical lObj1 in m_objects)
                 * {
                 *      foreach(Physical lObj2 in m_objects)
                 *      {
                 *           //intersection check code
                 *      }
                 * }
                 * 
                 * The problem is that this runs in O(N*N) time and that we're checking for collisions with objects which have already been checked.
                 * Imagine you have a set of four items: {1,2,3,4}
                 * You'd first check: {1} vs {1,2,3,4}
                 * Next, you'd check {2} vs {1,2,3,4}
                 * but we already checked {1} vs {2}, so it's a waste to check {2} vs. {1}. What if we could skip this check by removing {1}?
                 * We'd have a total of 4+3+2+1 collision checks, which equates to O(N(N+1)/2) time. If N is 10, we are already doing half as many collision checks as necessary.
                 * Now, we can't just remove an item at the end of the 2nd for loop since that would break the iterator in the first foreach loop, so we'd have to use a
                 * regular for(int i=0;i<size;i++) style loop for the first loop and reduce size each iteration. This works...but look at the for loop: we're allocating memory for
                 * two additional variables: i and size. What if we could figure out some way to eliminate those variables?
                 * So, who says that we have to start from the front of a list? We can start from the back end and still get the same end results. With this in mind,
                 * we can completely get rid of a for loop and use a while loop which has a conditional on the capacity of a temporary list being greater than 0.
                 * since we can poll the list capacity for free, we can use the capacity as an indexer into the list items. Now we don't have to increment an indexer either!
                 * The result is below.
                 */
                #endregion

                List<Physical> tmp = new List<Physical>(m_objects.Count);
                tmp.AddRange(m_objects);
                while (tmp.Count > 0)
                {
                    foreach (Physical lObj2 in tmp)
                    {
                        if (tmp[tmp.Count - 1] == lObj2 || (tmp[tmp.Count - 1].IsStationary && lObj2.IsStationary))
                            continue;
                        IntersectionRecord ir = tmp[tmp.Count - 1].Intersects(lObj2);
                        if (ir != null)
                        {
                            //ir.m_treeNode = this;
                            intersections.Add(ir);
                        }
                    }

                    //remove this object from the temp list so that we can run in O(N(N+1)/2) time instead of O(N*N)
                    tmp.RemoveAt(tmp.Count-1);
                }
            }

            //now, merge our local objects list with the parent objects list, then pass it down to all children.
            foreach (Physical lObj in m_objects)
                if (lObj.IsStationary == false)
                    parentObjs.Add(lObj);
            //parentObjs.AddRange(m_objects);

            //each child node will give us a list of intersection records, which we then merge with our own intersection records.
            for (int flags = m_activeNodes, index = 0; flags > 0; flags >>= 1, index++)
            {
                if ((flags & 1) == 1)
                {
                    if(m_childNode != null && m_childNode[index] != null)
                        intersections.AddRange(m_childNode[index].GetIntersection(parentObjs, type));
                }
            }
            
            return intersections;
        }


        /// <summary>
        /// This gives you a list of every intersection record created with the intersection ray
        /// </summary>
        /// <param name="intersectionRay">The ray to use for intersection</param>
        /// <returns></returns>
        public List<IntersectionRecord> AllIntersections(Ray intersectionRay)
        {

            return GetIntersection(intersectionRay);
        }

        /// <summary>
        /// This gives you the first object encountered by the intersection ray
        /// </summary>
        /// <param name="intersectionRay">The ray being used to intersect with</param>
        /// <param name="type">The type of the physical object to filter for</param>
        /// <returns></returns>
        public IntersectionRecord NearestIntersection(Ray intersectionRay, PhysicalType type = PhysicalType.ALL)
        {
            List<IntersectionRecord> intersections = GetIntersection(intersectionRay, type);

            IntersectionRecord nearest = null;

            foreach (IntersectionRecord ir in intersections)
            {
                if (nearest == null)
                {
                    nearest = ir;
                    continue;
                }

                if (ir.Distance < nearest.Distance)
                {
                    nearest = ir;
                }
            }

            return nearest;
        }

        /// <summary>
        /// This gives you a list of all intersections, filtered by a specific type of object
        /// </summary>
        /// <param name="intersectionRay">The ray to intersect with all objects</param>
        /// <param name="type">The type of physical object we're interested in intersecting with</param>
        /// <returns>A list of intersections of the specified type of geometry</returns>
        public List<IntersectionRecord> AllIntersections(Ray intersectionRay, PhysicalType type = PhysicalType.ALL)
        {
            List<IntersectionRecord> intersections = GetIntersection(intersectionRay, type);

            return intersections;
        }

        /// <summary>
        /// This gives you a list of all objects which [intersect or are contained within] the given frustum and meet the given object type
        /// </summary>
        /// <param name="region">The frustum to intersect with</param>
        /// <param name="type">The type of objects you want to filter</param>
        /// <returns>A list of intersection records for all objects intersecting with the frustum</returns>
        public List<IntersectionRecord> AllIntersections(BoundingFrustum region, PhysicalType type = PhysicalType.ALL)
        {
            return GetIntersection(region, type);
        }

        /// <summary>
        /// This gives you a list of all objects which intersect with the given bounding sphere which meet the filtering criteria
        /// </summary>
        /// <param name="region">The bounding sphere volume to test for collisions against</param>
        /// <param name="type">The particular type of objects you want to filter (Default: Include all objects)</param>
        /// <returns>A list of intersection records which contain intersection information.</returns>
        public List<IntersectionRecord> AllIntersections(BoundingSphere region, PhysicalType type = PhysicalType.ALL)
        {
            return GetIntersection(region, type);
        }

        /// <summary>
        /// This gives you a list of all objects in the tree which meet the given type filter criteria
        /// </summary>
        /// <param name="type">BITMASK: This is the object type to match on</param>
        /// <returns>A list of matched objects</returns>
        public List<Physical> AllObjects(PhysicalType type = PhysicalType.ALL)
        {

            if (type == PhysicalType.ALL)
                return m_allObjects;

            List<Physical> ret = new List<Physical>(m_allObjects.Count);

            //you know... if you were smart, you'd maintain a list for each object type or at least sort the objects.
            //then you could just merge lists together rather than going through each individual object and testing for a match.

            foreach (Physical p in m_allObjects)
            {
                int typeMatch = (int)(p.Type & type);   //untested

                if (typeMatch != 0)
                {
                    ret.Add(p);
                }
            }

            return ret;
        }

        #endregion

        #region Overrides
        public override string ToString()
        {
            string obj = (m_objects == null) ? "0" : m_objects.Count.ToString();
            if (_parent == null)
            {
                string children = "";
                if (m_childNode != null)
                {
                    for (int a = 0; a < 8; a++)
                    {
                        children += "[";
                        if (m_childNode[a] != null)
                            children += m_childNode[a].AllTreeObjects.Count;
                        else
                            children += "0";
                        children += "]";
                    }
                }
                
                return "Root : " + obj + " {" + children + "}";
            }
            else
            {
                if (this.HasChildren)
                {
                    string children = "";
                    for (int a = 0; a < 8; a++)
                    {
                        children += "[";
                        if (m_childNode[a] != null)
                            children += m_childNode[a].AllTreeObjects.Count;
                        else
                            children += "0";
                        children += "]";
                    }
                    return "Branch : " + obj + " {" + children + "}";
                }
                else
                {
                    return "Leaf : " + obj;
                }
            }
        }
        #endregion

        #region Accessors


        /// <summary>
        /// Gives you a list of every single object within the whole octree.
        /// </summary>
        /// <returns>A list of physical objects</returns>
        //public List<Physical> AllObjects
        //{
        //    //List<Physical> ret = new List<Physical>();

        //    //if(m_objects != null)
        //    //    ret.AddRange(m_objects);

        //    //if (HasChildren)
        //    //{
        //    //    for (int a = 0; a < 8; a++)
        //    //    {
        //    //        if (m_childNode[a] != null)
        //    //        {
        //    //            ret.AddRange(m_childNode[a].AllObjects());
        //    //        }
        //    //    }
        //    //}

        //    //return ret;
        //    get
        //    {
        //        return m_allObjects;
        //    }
        //}

        /// <summary>
        /// Gives you a list of all objects within this tree and all of its children
        /// </summary>
        public List<Physical> AllTreeObjects
        {
            get
            {
                List<Physical> ret = new List<Physical>();

                if (m_objects != null)
                    ret.AddRange(m_objects);

                if (HasChildren)
                {
                    for (int a = 0; a < 8; a++)
                    {
                        if (m_childNode[a] != null)
                        {
                            ret.AddRange(m_childNode[a].AllTreeObjects);
                        }
                    }
                }

                return ret;
            }
        }

        public int TotalObjects
        {
            get
            {
                return m_allObjects.Count;
            }
        }

        private bool IsRoot
        {
            //The root node is the only node without a parent.
            get { return _parent == null; }
        }

        private bool HasChildren
        {
            get
            {
                return m_activeNodes != 0;
            }
        }

        /// <summary>
        /// Returns true if this node tree and all children have no content
        /// </summary>
        private bool IsEmpty    //untested
        {
            get
            {
                return m_allObjects.Count == 0;
                //if (m_objects == null)
                //    return true;
                //if (m_objects != null && m_objects.Count != 0)
                //    return false;
                //else
                //{
                //    for (int a = 0; a < 8; a++)
                //    {
                //        //note that we have to do this recursively. 
                //        //Just checking child nodes for the current node doesn't mean that their children won't have objects.
                //        if (m_childNode[a] != null && !m_childNode[a].IsEmpty)
                //            return false;
                //    }

                //    return true;
                //}
            }
        }
        #endregion
    }
}