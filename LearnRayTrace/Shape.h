#pragma once
#include "Vec3.h"

class Shape
{
public:
	meta_name(Shape);
	Shape();
	~Shape();

	virtual ShadeInfo intersect(const Ray& ray) = 0;

	Vec3      _color;

	Material* _material = nullptr;
};

