#pragma once
#include "Shape.h"

class Ray;

class BBox
{
public:
	meta_name(BBox);

	virtual Vec3 center() const { return Vec3(); }
	virtual bool intersect(const Ray& ray) = 0;
};

