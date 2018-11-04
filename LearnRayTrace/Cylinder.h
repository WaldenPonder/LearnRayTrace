#pragma once
#include "Shape.h"

class Cylinder : public Shape
{
public:
	Cylinder();
	~Cylinder();

	virtual ShadeInfo intersect(const Ray& ray) override;
};

