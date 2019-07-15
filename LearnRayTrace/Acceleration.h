#pragma once
#include "util.h"
#include "BoundingBox.h"

class Acceleration
{
public:
	__class__(Acceleration);
	Acceleration();
	~Acceleration();

	virtual ShadeInfo intersect(const Ray& ray);
};
