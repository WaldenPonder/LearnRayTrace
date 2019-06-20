#pragma once
#include "Shape.h"

class Plane : public Shape
{
public:
	Plane();
	Plane(const Vec3& position, const  Vec3& normal);
	~Plane();

	virtual ShadeInfo intersect(const Ray& ray) override;

public:
	Vec3 _position;
	Vec3 _normal;

	float x_min = 0;
	float x_max = 0;
	float y_min = 0;
	float y_max = 0;
	float z_min = 0;
	float z_max = 0;
};

