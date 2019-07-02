#include "BVH.h"
#include "BoundingBox.h"
#include "MeshObject.h"
#include "World.h"
#include "stdafx.h"
#include <array>

struct OctreeNode {
    Extent pbbox;
	std::vector<const Extent *> nodeExtentList;
    std::vector< MeshObject* >   meshs;
    std::array< OctreeNode*, 8 > children = { nullptr };
    bool                         isLeaf   = true;
};

struct BVH::Octree {

	void insert(OctreeNode*& node, const Extent* extent, const BoundingBox& bbox, uint32_t depth);
	void build(OctreeNode& node, BoundingBox& bbox);

    BoundingBox                   bbox_;
    Extent           pbbox_;
    vector< Extent > pbboxList_;  //所有的pbb集合
    OctreeNode                    root_;
};

void BVH::Octree::insert(OctreeNode*& node, const Extent* extent, const BoundingBox& bbox, uint32_t depth)
{
	if (node->isLeaf) {
		if (node->nodeExtentList.size() == 0 || depth == 16) {
			node->nodeExtentList.push_back(extent);
		}
		else {
			node->isLeaf = false;
			// Re-insert extents held by this node
			while (node->nodeExtentList.size()) {
				insert(node, node->nodeExtentList.back(), bbox, depth);
				node->nodeExtentList.pop_back();
			}
			// Insert new extent
			insert(node, extent, bbox, depth);
		}
	}
	else {
		// Need to compute in which child of the current node this extents should
		// be inserted into
		Vec3 extentCentroid = extent->center();
		Vec3 nodeCentroid = (bbox[0] + bbox[1]) * 0.5;
		BoundingBox childBBox;
		uint8_t childIndex = 0;
		// x-axis
		if (extentCentroid.x > nodeCentroid.x) {
			childIndex = 4;
			childBBox[0].x = nodeCentroid.x;
			childBBox[1].x = bbox[1].x;
		}
		else {
			childBBox[0].x = bbox[0].x;
			childBBox[1].x = nodeCentroid.x;
		}
		// y-axis
		if (extentCentroid.y > nodeCentroid.y) {
			childIndex += 2;
			childBBox[0].y = nodeCentroid.y;
			childBBox[1].y = bbox[1].y;
		}
		else {
			childBBox[0].y = bbox[0].y;
			childBBox[1].y = nodeCentroid.y;
		}
		// z-axis
		if (extentCentroid.z > nodeCentroid.z) {
			childIndex += 1;
			childBBox[0].z = nodeCentroid.z;
			childBBox[1].z = bbox[1].z;
		}
		else {
			childBBox[0].z = bbox[0].z;
			childBBox[1].z = nodeCentroid.z;
		}

		// Create the child node if it doesn't exsit yet and then insert the extents in it
		if (node->children[childIndex] == nullptr)
			node->children[childIndex] = new OctreeNode;
		insert(node->children[childIndex], extent, childBBox, depth + 1);
	}
}

void BVH::Octree::build(OctreeNode& node, BoundingBox& bbox)
{

}

BVH::BVH() {}

BVH::~BVH() {}

void BVH::build() {
    octree_ = new Octree;

    for ( Shape* shape : World::Instance()->_shapes ) {
        if ( shape->className() == MeshObject::class_name() ) {
            MeshObject*         mesh = dynamic_cast< MeshObject* >( shape );
            Extent pbb( mesh );
            octree_->pbboxList_.push_back( pbb );
            octree_->pbbox_.extendBy( pbb );
        }
    }

    //--------------------------------------------------BVH::Octree::bbox_
    float xDiff   = octree_->pbbox_.slabs_[ 0 ][ 1 ] - octree_->pbbox_.slabs_[ 0 ][ 0 ];
    float yDiff   = octree_->pbbox_.slabs_[ 1 ][ 1 ] - octree_->pbbox_.slabs_[ 1 ][ 0 ];
    float zDiff   = octree_->pbbox_.slabs_[ 2 ][ 1 ] - octree_->pbbox_.slabs_[ 2 ][ 0 ];
    float maxDiff = max( xDiff, max( yDiff, zDiff ) );
    Vec3  minPlusMax( octree_->pbbox_.slabs_[ 0 ][ 0 ] + octree_->pbbox_.slabs_[ 0 ][ 1 ], octree_->pbbox_.slabs_[ 1 ][ 0 ] + octree_->pbbox_.slabs_[ 1 ][ 1 ],
                     octree_->pbbox_.slabs_[ 2 ][ 0 ] + octree_->pbbox_.slabs_[ 2 ][ 1 ] );
    octree_->bbox_._minPt = ( minPlusMax - Vec3( maxDiff ) ) * 0.5;
    octree_->bbox_._maxPt = ( minPlusMax + Vec3( maxDiff ) ) * 0.5;


	//--------------------------------------------------insert and building
	for (auto& pbb : octree_->pbboxList_)
	{
		octree_->insert(pbb);
	}

	octree_->build(octree_->root_, octree_->bbox_);
}
