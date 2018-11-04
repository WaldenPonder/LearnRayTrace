#include "stdafx.h"
#include "World.h"
#include "Sphere.h"
#include "Plane.h"
#include "ShadeInfo.h"
#include "Material.h"
#include "Ray.h"
#include "Phong.h"
#include "Cylinder.h"

World::World() : _camera(*this)
{
	buildScene();
}


World::~World()
{
}

void World::buildScene()
{
	Sphere* sphere = new Sphere(Vec3(0, 0, -5), .5);
	Phong* p1 = new Phong(g::Red);
	sphere->_material = p1;
	_shapes.push_back(sphere);

	Plane* plane = new Plane(Vec3(0, -1, 0), Vec3(0, 1, 0));
	Phong* p2 = new Phong(g::Blue);
	plane->_material = p2;
	_shapes.push_back(plane);

	plane = new Plane(Vec3(-1, 0, 0), Vec3(1, 0, 0));
	plane->_material = new Phong(g::Green);
	_shapes.push_back(plane);

	plane = new Plane(Vec3(0, 0, -1), Vec3(0, 0, 1));
	plane->_material = new Phong(g::Yellow);
	_shapes.push_back(plane);

	plane = new Plane(Vec3(1, 0, 0), Vec3(-1, 0, 0));
	plane->_material = new Phong(g::Cyan);
	_shapes.push_back(plane);

	//Cylinder* cy = new Cylinder;
	//cy->_material = new Phong(g::Bronze);
	//_shapes.push_back(cy);

}

ShadeInfo World::intersection(Ray& ray)
{
	ShadeInfo value;

	for (Shape* s : _shapes)
	{
		value = s->intersect(ray);
		if (value.shape)
		{
			value.color = value.material->shade(value);
			break;
		}
	}

	return value;
}
