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

	struct Octree;
	Octree* octree_;
};