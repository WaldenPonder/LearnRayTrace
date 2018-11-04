#pragma once
#include "Shape.h"

class Sphere : public Shape
{
public:
	Sphere();
	Sphere(const Vec3& center, float radius);
	~Sphere();

	virtual ShadeInfo intersect(const Ray& ray) override;

public:
	Vec3 _center;
	float _radius;
};

