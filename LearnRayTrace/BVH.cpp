#include "stdafx.h"
#include "BVH.h"
#include "BoundingBox.h"
#include "MeshObject.h"
#include "World.h"
#include <array>
#include <queue>

struct OctreeNode
{
	Extent					   nodeExtent;
	std::vector<const Extent*> nodeExtentList;
	std::array<OctreeNode*, 8> children = { nullptr };
	bool					   isLeaf   = true;
};

struct QueueElement
{
	const OctreeNode* node;  // octree node held by this element in the queue
	float			  t;	 // distance from the ray origin to the extents of the node
	QueueElement(const OctreeNode* n, float tn) : node(n), t(tn) {}
	// priority_queue behaves like a min-heap
	friend bool operator<(const QueueElement& a, const QueueElement& b)
	{
		return a.t > b.t;
	}
};

struct BVH::Octree
{
	void insert(OctreeNode* node, const Extent* extent, const BoundingBox& bbox, uint32_t depth);
	void build(OctreeNode* node, const BoundingBox& bbox);

	BoundingBox	bbox;
	Extent		   nodeExtent;
	vector<Extent> allExtent;  //所有的extent集合
	OctreeNode	 root;
};

void BVH::Octree::insert(OctreeNode* node, const Extent* extent, const BoundingBox& bbox, uint32_t depth)
{
	if (node->isLeaf)
	{
		if (node->nodeExtentList.size() == 0 || depth == 16)
		{
			node->nodeExtentList.push_back(extent);
		}
		else
		{
			node->isLeaf = false;
			// Re-insert extents held by this node
			while (node->nodeExtentList.size())
			{
				insert(node, node->nodeExtentList.back(), bbox, depth);
				node->nodeExtentList.pop_back();
			}
			// Insert new extent
			insert(node, extent, bbox, depth);
		}
	}
	else
	{
		// Need to compute in which child of the current node this extents should
		// be inserted into
		Vec3		extentCentroid = extent->center();
		Vec3		nodeCentroid   = (bbox[0] + bbox[1]) * 0.5;
		BoundingBox childBBox;
		uint8_t		childIndex = 0;
		// x-axis
		if (extentCentroid.x() > nodeCentroid.x())
		{
			childIndex		 = 4;
			childBBox[0].x() = nodeCentroid.x();
			childBBox[1].x() = bbox[1].x();
		}
		else
		{
			childBBox[0].x() = bbox[0].x();
			childBBox[1].x() = nodeCentroid.x();
		}
		// y-axis
		if (extentCentroid.y() > nodeCentroid.y())
		{
			childIndex += 2;
			childBBox[0].y() = nodeCentroid.y();
			childBBox[1].y() = bbox[1].y();
		}
		else
		{
			childBBox[0].y() = bbox[0].y();
			childBBox[1].y() = nodeCentroid.y();
		}
		// z-axis
		if (extentCentroid.z() > nodeCentroid.z())
		{
			childIndex += 1;
			childBBox[0].z() = nodeCentroid.z();
			childBBox[1].z() = bbox[1].z();
		}
		else
		{
			childBBox[0].z() = bbox[0].z();
			childBBox[1].z() = nodeCentroid.z();
		}

		// Create the child node if it doesn't exsit yet and then insert the extents in it
		if (node->children[childIndex] == nullptr)
			node->children[childIndex] = new OctreeNode;
		insert(node->children[childIndex], extent, childBBox, depth + 1);
	}
}

void BVH::Octree::build(OctreeNode* node, const BoundingBox& bbox)
{
	if (node->isLeaf)
	{
		for (const auto& e : node->nodeExtentList)
		{
			node->nodeExtent.extendBy(*e);
		}
	}
	else
	{
		for (uint8_t i = 0; i < 8; ++i)
		{
			if (node->children[i])
			{
				BoundingBox childBBox;
				Vec3f		centroid = bbox.center();
				// x-axis
				childBBox[0].x() = (i & 4) ? centroid.x() : bbox[0].x();
				childBBox[1].x() = (i & 4) ? bbox[1].x() : centroid.x();
				// y-axis
				childBBox[0].y() = (i & 2) ? centroid.y() : bbox[0].y();
				childBBox[1].y() = (i & 2) ? bbox[1].y() : centroid.y();
				// z-axis
				childBBox[0].z() = (i & 1) ? centroid.z() : bbox[0].z();
				childBBox[1].z() = (i & 1) ? bbox[1].z() : centroid.z();

				// Inspect child
				build(node->children[i], childBBox);

				// Expand extents with extents of child
				node->nodeExtent.extendBy(node->children[i]->nodeExtent);
			}
		}
	}
}

BVH::BVH() { init(); }

BVH::~BVH() {}

void BVH::init()
{
	octree_ = new Octree;

	for (Shape* shape : World::Instance()->shapes_)
	{
		if (shape->className() == MeshObject::class_name())
		{
			MeshObject* mesh = dynamic_cast<MeshObject*>(shape);
			Extent		extent(mesh);
			octree_->allExtent.push_back(extent);
			octree_->nodeExtent.extendBy(extent);
		}
	}

	//--------------------------------------------------BVH::Octree::bbox_
	float xDiff   = octree_->nodeExtent.slabs_[0][1] - octree_->nodeExtent.slabs_[0][0];
	float yDiff   = octree_->nodeExtent.slabs_[1][1] - octree_->nodeExtent.slabs_[1][0];
	float zDiff   = octree_->nodeExtent.slabs_[2][1] - octree_->nodeExtent.slabs_[2][0];
	float maxDiff = max(xDiff, max(yDiff, zDiff));
	Vec3  minPlusMax(octree_->nodeExtent.slabs_[0][0] + octree_->nodeExtent.slabs_[0][1], octree_->nodeExtent.slabs_[1][0] + octree_->nodeExtent.slabs_[1][1],
					 octree_->nodeExtent.slabs_[2][0] + octree_->nodeExtent.slabs_[2][1]);
	octree_->bbox._minPt = (minPlusMax - Vec3(maxDiff)) * 0.5;
	octree_->bbox._maxPt = (minPlusMax + Vec3(maxDiff)) * 0.5;

	//--------------------------------------------------insert and building
	for (auto& extent : octree_->allExtent)
	{
		octree_->insert(&octree_->root, &extent, octree_->bbox, 0);
	}

	octree_->build(&octree_->root, octree_->bbox);
}

ShadeInfo BVH::intersect(const Ray& ray)
{
	ShadeInfo		  info;
	float			  tHit			  = FLT_MAX;
	const MeshObject* intersectedMesh = nullptr;

	uint8_t planeIndex;
	float   tNear = 0, tFar = FLT_MAX;  // tNear, tFar for the intersected extents
	if (!octree_->root.nodeExtent.intersect(ray) || tFar < 0)
		return info;
	tHit = tFar;
	std::priority_queue<QueueElement> queue;
	queue.push(QueueElement(&octree_->root, 0));
	while (!queue.empty() && queue.top().t < tHit)
	{
		const OctreeNode* node = queue.top().node;
		queue.pop();
		if (node->isLeaf)
		{
			for (const auto& e : node->nodeExtentList)
			{
				ShadeInfo tmpInfo = e->mesh_->intersect(ray);
				if (tmpInfo.valid() && tmpInfo.dis < tHit)
				{
					info	 = tmpInfo;
					info.dis = tHit = tmpInfo.dis;
					info.setShape(e->mesh_);
				}
			}
		}
		else
		{
			for (uint8_t i = 0; i < 8; ++i)
			{
				if (node->children[i] != nullptr)
				{
					float tNearChild = 0, tFarChild = tFar;
					if (node->children[i]->nodeExtent.intersect(ray.precomputedNumerator, ray.precomputedDenominator, tNearChild, tFarChild, planeIndex))
					{
						float t = (tNearChild < 0 && tFarChild >= 0) ? tFarChild : tNearChild;
						queue.push(QueueElement(node->children[i], t));
					}
				}
			}
		}
	}

	return info;
}
