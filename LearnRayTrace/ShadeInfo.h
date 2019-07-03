#pragma once
#include "common.h"
#include "Ray.h"

//  #define  USING_COUT

class Shape;
class World;
class Material;

class ShadeInfo
{
public:
	meta_name(ShadeInfo);
	ShadeInfo();

	float     distance = 0;
	Shape*    shape = nullptr;
	int       depth = 0;
	Vec3      normal;
	Vec3      position;

	World*    world = nullptr;
	Vec3      color;
	Ray       ray;
	float u = 0;
	float v = 0;

	Material* material = nullptr;

	void setShape(Shape* s);

	bool valid();
};
