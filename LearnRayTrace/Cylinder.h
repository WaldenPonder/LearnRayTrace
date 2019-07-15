#pragma once
#include "Shape.h"

class Cylinder : public Shape
{
public:
	__class__(Cylinder);
	Cylinder();
	~Cylinder();

	virtual ShadeInfo intersect(const Ray& ray) override;
};

