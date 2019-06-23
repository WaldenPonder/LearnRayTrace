#include "stdafx.h"
#include "World.h"
#include "Sphere.h"
#include "Plane.h"
#include "ShadeInfo.h"
#include "Material.h"
#include "Ray.h"
#include "Matte.h"
#include "Cylinder.h"
#include "Box.h"
#include "GlossyReflector.h"
#include "Emissive.h"
#include "PerfectReflect.h"


#define MATERIAL Matte

World::World() : _camera(*this)
{
	_max_depth = 2;
	buildScene();
}


World::~World()
{
}




//--------------------------------------------------Y 向上， 右手
void World::buildScene()
{
	Plane* plane = nullptr;

	plane = new Plane(Vec3(0, 2, 0), Vec3(0, -1, 0)); //上
	plane->_material = new MATERIAL(g::Cyan);
	_shapes.push_back(plane);

	plane = new Plane(Vec3(0, 2 - 1e-5, 0), Vec3(0, -1, 0)); //灯
	plane->_material = new Emissive;
	plane->x_min = -.5, plane->x_max = .5;
	plane->z_min = -5, plane->z_max = -4;
	_shapes.push_back(plane);

	plane = new Plane(Vec3(0, -2, 0), Vec3(0, 1, 0)); //下
	plane->_material = new MATERIAL(g::Blue);
	_shapes.push_back(plane);

	plane = new Plane(Vec3(-2, 0, 0), Vec3(1, 0, 0)); //左
	plane->_material = new MATERIAL(g::Green);
	_shapes.push_back(plane);

	plane = new Plane(Vec3(0, 0, -6), Vec3(0, 0, 1)); //后
	plane->_material = new MATERIAL(g::Yellow);
	_shapes.push_back(plane);

	plane = new Plane(Vec3(2, 0, 0), Vec3(-1, 0, 0)); //右
	plane->_material = new MATERIAL(g::Cyan);
	_shapes.push_back(plane);

	//--------------------------------------------------Sphere
	Sphere* sphere = new Sphere(Vec3(0, -1.5, -5), .5);
	sphere->_material = new MATERIAL(g::Red);
	_shapes.push_back(sphere);

	//sphere = new Sphere(Vec3(-1, -1.5, -5), .5);
	//sphere->_material = new MATERIAL(g::Red);
	//_shapes.push_back(sphere);

	//sphere = new Sphere(Vec3(1, -1.5, -5), .5);
	//sphere->_material = new MATERIAL(g::Red);
	//_shapes.push_back(sphere);

	//sphere = new Sphere(Vec3(0, -1.5, -4), .5);
	//sphere->_material = new MATERIAL(g::Yellow);
	//_shapes.push_back(sphere);
}

ShadeInfo World::intersection(const Ray& ray)
{
	ShadeInfo info;

	for (Shape* s : _shapes)
	{
		ShadeInfo value = s->intersect(ray);
		if (value.shape)
		{
			if (value.distance < info.distance)
			{
				info = value;
			}
		}
	}

	return info;
}

//返回最终颜色
Color World::trace_ray(const Ray ray, int depth)
{
	ShadeInfo info(this->intersection(ray));

	if (info.valid())
	{
		info.depth = depth;
		info.ray = ray;

		Color c = info.material->shade(info);
		return c;
	}

	return _bgColor;
}
