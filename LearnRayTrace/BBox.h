#pragma once
#include "Shape.h"

class BBox
{
public:
	meta_name(BBox);
	BBox();
	~BBox();

	virtual bool intersect(const Ray& ray) = 0;
};

