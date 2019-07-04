#include "stdafx.h"
#include "World.h"
#include "Plane.h"
#include "Checker.h"
#include "Sphere.h"
#include "Emissive.h"
#include "Matte.h"
#include "MeshObject.h"
#include "Acceleration.h"

#undef max
#undef min
#include <random>

#define MATERIAL Matte

World::World() : camera_(*this)
{
	max_depth_ = 2;
	buildScene2();
}

World::~World() {}

//--------------------------------------------------Y 向上， 右手
void World::buildScene1()
{
	Plane* plane = nullptr;

	plane			 = new Plane(Vec3(0, 2, 0), Vec3(0, -1, 0));  //上
	plane->material_ = new MATERIAL(g::Cyan);
	shapes_.push_back(plane);

	plane			 = new Plane(Vec3(0, 2 - 1e-5, 0), Vec3(0, -1, 0));  //灯
	plane->material_ = new Emissive;
	plane->x_min = -.5, plane->x_max = .5;
	plane->z_min = -5, plane->z_max = -4;
	shapes_.push_back(plane);

	plane			 = new Plane(Vec3(0, -2, 0), Vec3(0, 1, 0));  //下
	plane->material_ = new MATERIAL(g::Blue);
	shapes_.push_back(plane);

	plane			 = new Plane(Vec3(-2, 0, 0), Vec3(1, 0, 0));  //左
	plane->material_ = new MATERIAL(g::Green);
	shapes_.push_back(plane);

	plane			 = new Plane(Vec3(0, 0, -6), Vec3(0, 0, 1));  //后
	plane->material_ = new MATERIAL(g::Yellow);
	shapes_.push_back(plane);

	plane			 = new Plane(Vec3(2, 0, 0), Vec3(-1, 0, 0));  //右
	plane->material_ = new MATERIAL(g::Cyan);
	shapes_.push_back(plane);

	//--------------------------------------------------Sphere
	Sphere* sphere	= new Sphere(Vec3(0, -1.5, -5), .5);
	sphere->material_ = new MATERIAL(g::Red);
	shapes_.push_back(sphere);

	sphere			  = new Sphere(Vec3(-1, -1.5, -5), .5);
	sphere->material_ = new MATERIAL(g::Red);
	shapes_.push_back(sphere);

	sphere			  = new Sphere(Vec3(1, -1.5, -5), .5);
	sphere->material_ = new MATERIAL(g::Red);
	shapes_.push_back(sphere);

	sphere			  = new Sphere(Vec3(0, -1.5, -4), .5);
	sphere->material_ = new MATERIAL(g::Yellow);
	shapes_.push_back(sphere);
}

//--------------------------------------------------Y 向上， 右手
void World::buildScene2()
{
	Plane* plane = nullptr;

	plane			 = new Plane(Vec3(0, -25, 0), Vec3(0, 1, 0));
	plane->material_ = new Checker();
	shapes_.push_back(plane);

	//--------------------------------------------------Sphere
	Shape* shape = new Sphere(Vec3(0, -1.5, -11), .5);
	// shape->_material = new MATERIAL(g::Red);
	//_shapes.push_back(shape);

	// shape = new Sphere(Vec3(-1, -1.5, -10), .5);
	// shape->_material = new MATERIAL(g::Red);
	//_shapes.push_back(shape);

	// shape = new Sphere(Vec3(1, -1.5, -9), .5);
	// shape->_material = new MATERIAL(g::Red);
	//_shapes.push_back(shape);

	// shape = new Sphere(Vec3(0, -1.5, -8), .5);
	// shape->_material = new MATERIAL(g::Yellow);
	//_shapes.push_back(shape);

	std::default_random_engine			  gen(time(NULL));
	std::uniform_real_distribution<float> distri_x(-40, 40);
	std::uniform_real_distribution<float> distri_z(-300, -30);

	int   i = 0;
	float x, z;

	static Mesh bunyMesh("../3rd/bunny.objt");

	for (i = 0; i < 50; i++)
	{
		MeshObject* bunny = new MeshObject(bunyMesh);

		x				 = distri_x(gen);
		z				 = distri_z(gen);
		
		bunny->matrix_   = Matrix::rotate(z, Vec3(0,1,0)) * Matrix::scale(50) * Matrix::translate(x, -10, z);
		bunny->material_ = new MATERIAL(g::Blue);
		shapes_.push_back(bunny);
	}
}

ShadeInfo World::intersection(const Ray& ray)
{
	//if (accel_)
	//	return accel_->intersect(ray);

	ShadeInfo info;

	for (Shape* s : shapes_)
	{
		ShadeInfo value = s->intersect(ray);
		if (value.shape)
		{
			if (value.dis < info.dis)
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
		info.ray   = ray;

		Color c = info.material->shade(info);
		return c;
	}

	return bgColor_;
}

Color World::trace_ray_direct(const Ray ray, int depth)
{
	ShadeInfo info(this->intersection(ray));

	if (info.valid())
	{
		info.depth = depth;
		info.ray   = ray;

		Color c = info.material->getColor(info);
		return c;
	}

	return bgColor_;
}

void World::max_to_one(Color& c) const
{
	if (c.r() > 1 || c.g() > 1 || c.b() > 1)
	{
		float maxV = std::max({ kEpsilon, c.r(), c.g(), c.b() });

		c /= maxV;
	}
}

void World::clamp_to_color(Color& c, const Color& clamto /*= g::Red*/) const
{
	if (c.r() > 1 || c.g() > 1 || c.b() > 1)
	{
		c = clamto;
	}
}
