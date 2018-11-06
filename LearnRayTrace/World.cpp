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

World::World() : _camera(*this)
{
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
	plane->_material = new Phong(g::Cyan);
	_shapes.push_back(plane);

	plane = new Plane(Vec3(0, -2, 0), Vec3(0, 1, 0)); //下
	plane->_material = new Phong(g::Blue);
	_shapes.push_back(plane);

	plane = new Plane(Vec3(-2, 0, 0), Vec3(1, 0, 0)); //左
	plane->_material = new Phong(g::Green);
	_shapes.push_back(plane);

	plane = new Plane(Vec3(0, 0, -6), Vec3(0, 0, 1)); //后
	plane->_material = new Phong(g::Yellow);
	_shapes.push_back(plane);

	plane = new Plane(Vec3(2, 0, 0), Vec3(-1, 0, 0)); //右
	plane->_material = new Phong(g::Cyan);
	_shapes.push_back(plane);

	Cylinder* cy = new Cylinder;
	cy->_material = new Phong(g::Bronze);
	//_shapes.push_back(cy);

	Sphere* sphere = new Sphere(Vec3(0, 0, -5), .5);
	sphere->_material = new Phong(g::Red);
	_shapes.push_back(sphere);

	Box* box = new Box(Vec3(3, .5, -30), 2);
	box->_material = new Phong(g::Cyan);
	//_shapes.push_back(box);
}

ShadeInfo World::intersection(Ray& ray)
{
	ShadeInfo result;

	for (Shape* s : _shapes)
	{
		ShadeInfo value = s->intersect(ray);
		if (value.shape)
		{
			if (value.distance < result.distance)
			{
				result = value;
				result.color = result.material->shade(result);
			}
		}
	}

	return result;
}
