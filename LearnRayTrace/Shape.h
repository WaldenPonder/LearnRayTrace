#pragma once
#include "Vec3.h"

class Shape : public InstancePool<Shape>, public Object
{
public:
	__class__(Shape);
	Shape();
	~Shape();

	virtual ShadeInfo intersect(const Ray& ray) = 0;

	Vec3      color_;

	Material* material_ = nullptr;
	Shape* parent_ = nullptr;
};

