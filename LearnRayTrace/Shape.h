#pragma once
#include "Vec3.h"
#include "Ray.h"
#include "ShadeInfo.h"

class Shape;
class World;
class Material;

class Shape
{
public:
	Shape();
	~Shape();

	virtual ShadeInfo intersect(const Ray& ray) = 0;

	Vec3      _color;

	Material* _material = nullptr;
};

