#pragma once
#include "util.h"
#include "BoundingBox.h"

class Acceleration
{
public:
	meta_name(Acceleration);
	Acceleration();
	~Acceleration();

	virtual ShadeInfo intersect(const Ray& ray) { return ShadeInfo(); }
};
