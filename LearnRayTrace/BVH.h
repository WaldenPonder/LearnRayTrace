#pragma once
#include "Acceleration.h"
#include "Extent.h"

class BVH : public Acceleration
{
public:
	__class__(BVH);
	BVH();
	virtual ~BVH();

	void init();
	virtual ShadeInfo intersect(const Ray& ray) override;

	struct Octree;
	Octree* octree_;
};