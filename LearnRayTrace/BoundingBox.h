#pragma once
#include "Shape.h"

class BoundingBox
{
public:
	BoundingBox();
	~BoundingBox();

	void reset();

	bool valid() const;

	void expandBy(const Vec3& pt);

	bool intersect(const Ray& ray);

	Vec3 _minPt = Vec3(FLT_MAX);
	Vec3 _maxPt = Vec3(-FLT_MAX);
};