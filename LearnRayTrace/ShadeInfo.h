#pragma once
#include "Vec3.h"
#include "Ray.h"

class Shape;
class World;
class Material;

class ShadeInfo
{
public:
	ShadeInfo();

	float     distance = 0;
	Shape*    shape = nullptr;

	Vec3      normal;
	Vec3      position;

	World*    world = nullptr;
	Vec3      color;
	Ray       ray;

	Material* material = nullptr;

	void setShape(Shape* s);

	operator bool() const;
};
