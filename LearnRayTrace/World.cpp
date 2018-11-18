#include "stdafx.h"
#include "World.h"
#include "Sphere.h"
#include "Plane.h"
#include "ShadeInfo.h"
#include "Material.h"
#include "Ray.h"
#include "Phong.h"
#include "Cylinder.h"
#include "Box.h"
#include "GlossyReflector.h"

World::World() : _camera(*this)
{
	_max_depth = 4;
	buildScene();
}


World::~World()
{
}

//Y 向上， 右手
void World::buildScene()
{
	Plane* plane = nullptr;
	
	plane = new Plane(Vec3(0, 2, 0), Vec3(0, -1, 0)); //上
	plane->_material = new GlossyReflector(g::Cyan);
	_shapes.push_back(plane);

	plane = new Plane(Vec3(0, -2, 0), Vec3(0, 1, 0)); //下
	plane->_material = new GlossyReflector(g::Blue);
	_shapes.push_back(plane);

	plane = new Plane(Vec3(-2, 0, 0), Vec3(1, 0, 0)); //左
	plane->_material = new GlossyReflector(g::Green);
	_shapes.push_back(plane);

	plane = new Plane(Vec3(0, 0, -6), Vec3(0, 0, 1)); //后
	plane->_material = new GlossyReflector(g::Yellow);
	_shapes.push_back(plane);

	plane = new Plane(Vec3(2, 0, 0), Vec3(-1, 0, 0)); //右
	plane->_material = new GlossyReflector(g::Cyan);
	_shapes.push_back(plane);

	Cylinder* cy = new Cylinder;
	cy->_material = new GlossyReflector(g::Bronze);
	//_shapes.push_back(cy);

#pragma region Sphere

	Sphere* sphere = new Sphere(Vec3(0, -1.5, -5), .5);
	sphere->_material = new GlossyReflector(g::Red);
	_shapes.push_back(sphere);

	sphere = new Sphere(Vec3(-1, -1.5, -5), .5);
	sphere->_material = new GlossyReflector(g::Red);
	_shapes.push_back(sphere);

	sphere = new Sphere(Vec3(1, -1.5, -5), .5);
	sphere->_material = new GlossyReflector(g::Red);
	_shapes.push_back(sphere);

	sphere = new Sphere(Vec3(0, -1.5, -4), .5);
	sphere->_material = new GlossyReflector(g::Yellow);
	_shapes.push_back(sphere);

#pragma endregion Sphere

	Box* box = new Box(Vec3(3, .5, -30), 2);
	box->_material = new GlossyReflector(g::Cyan);
	//_shapes.push_back(box);
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

Color World::trace_ray(const Ray ray, int depth)
{
	if (depth > _max_depth)
	{
		return Vec3(1);
		//return g::Black;
	}
	else
	{
		ShadeInfo info(this->intersection(ray));

		if (info)
		{
			info.depth = depth;
			info.ray = ray;

			return info.material->shade(info);
		}		
	}

	return _bgColor;
}
