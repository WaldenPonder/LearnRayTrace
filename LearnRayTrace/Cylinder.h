#pragma once
#include "Shape.h"

class Cylinder : public Shape
{
public:
	meta_name(Cylinder);
	Cylinder();
	~Cylinder();

	virtual ShadeInfo intersect(const Ray& ray) override;
};

