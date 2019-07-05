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

void RANDOM_PT(vector<float>& xVec, vector<float>& zVec)
{
	std::default_random_engine			  gen(time(NULL));
	std::uniform_real_distribution<float> distri_x(-40, 40);
	std::uniform_real_distribution<float> distri_z(-300, -30);

#if 0

	std::ofstream OFILE;
	OFILE.open("../3rd/random_pt.txt");

	int i = 0;
	float x, z;

	for (i = 0; i < 5000; i++)
	{
		x = distri_x(gen);
		z = distri_z(gen);
		OFILE << x << "\t" << z << "\n";
	}
	OFILE.close();

#else

	std::ifstream IFILE;
	IFILE.open("../3rd/random_pt.txt");

	int   i = 0;
	float x, z;

	while (!IFILE.eof())
	{
		IFILE >> x >> z;
		xVec.push_back(x);
		zVec.push_back(z);
	}

	IFILE.close();

#endif
}

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
	shapes_ << plane;

	int   i = 0;
	float x, z;

	static Mesh bunyMesh("../3rd/bunny.objt");

	vector<float> xVec, zVec;
	RANDOM_PT(xVec, zVec);

	//-------------------------------------------------bunny
	for (i = 0; i < 1; i++)
	{
		MeshObject* bunny = new MeshObject(bunyMesh);

		bunny->matrix_   = Matrix::rotate(0, Vec3(0, 1, 0)) * Matrix::scale(50) * Matrix::translate(xVec[i], -10, zVec[i]);
		bunny->material_ = new MATERIAL(g::Blue);
		shapes_ << bunny;
	}

	//-------------------------------------------------venusm
	static Mesh venusmMesh("../3rd/venusm.objt");
	MeshObject* venusm = new MeshObject(venusmMesh);

	venusm->matrix_ = Matrix::rotate(0, Vec3(0, 1, 0)) * Matrix::scale(.01) * Matrix::translate(0, -10, -100);
	venusm->material_ = new MATERIAL(g::Blue);
	shapes_ << venusm;
}

ShadeInfo World::intersection(const Ray& ray) const
{
	ShadeInfo info;

	if (accel_)
	{
		info			  = accel_->intersect(ray);
		ShadeInfo tmpInfo = intersection_without_meshobject(ray);

		if (tmpInfo.valid() && tmpInfo.dis < info.dis)
		{
			info = tmpInfo;
		}

		return info;
	}

	for (Shape* s : shapes_)
	{
		ShadeInfo value = s->intersect(ray);
		if (value.valid())
		{
			if (value.dis < info.dis)
			{
				info = value;
			}
		}
	}

	return info;
}

ShadeInfo World::intersection_without_meshobject(const Ray& ray) const
{
	ShadeInfo info;

	for (Shape* s : shapes_)
	{
		if (s->className() == MeshObject::class_name())
			continue;

		ShadeInfo value = s->intersect(ray);
		if (value.valid())
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
