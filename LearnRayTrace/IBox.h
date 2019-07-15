#pragma once
#include "Shape.h"

class Ray;

class IBox
{
public:
	__class__(IBox);

	virtual Vec3 center() const { return Vec3(); }
	virtual bool intersect(const Ray& ray) = 0;
};

