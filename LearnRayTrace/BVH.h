#pragma once
#include "Acceleration.h"
#include "Extent.h"

class BVH : public Acceleration
{
public:
	meta_name(BVH);
	BVH();
	virtual ~BVH();

	void build();
	virtual bool intersect(const Ray& ray) override;

	struct Octree;
	Octree* octree_;
};