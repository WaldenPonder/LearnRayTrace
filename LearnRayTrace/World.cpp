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
#include "Light.h"
#include <assert.h>

#define MATERIAL Matte

begin_name_space(g);

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

end_name_space

World::World() : camera_(*this)
{
	max_depth_ = 2;
	buildScene2();
}

World::~World() {}

//--------------------------------------------------Y ���ϣ� ����
void World::buildScene1()
{
	Plane* plane = nullptr;

	plane			 = new Plane(Vec3(0, 2, 0), Vec3(0, -1, 0));  //��
	plane->material_ = new MATERIAL;
	shapes_.push_back(plane);

	plane			 = new Plane(Vec3(0, 2 - 1e-5, 0), Vec3(0, -1, 0));  //��
	plane->material_ = new Emissive;
	plane->x_min = -.5, plane->x_max = .5;
	plane->z_min = -5, plane->z_max = -4;
	shapes_.push_back(plane);

	plane			 = new Plane(Vec3(0, -2, 0), Vec3(0, 1, 0));  //��
	plane->material_ = new MATERIAL;
	shapes_.push_back(plane);

	plane			 = new Plane(Vec3(-2, 0, 0), Vec3(1, 0, 0));  //��
	plane->material_ = new MATERIAL;
	shapes_.push_back(plane);

	plane			 = new Plane(Vec3(0, 0, -6), Vec3(0, 0, 1));  //��
	plane->material_ = new MATERIAL;
	shapes_.push_back(plane);

	plane			 = new Plane(Vec3(2, 0, 0), Vec3(-1, 0, 0));  //��
	plane->material_ = new MATERIAL;
	shapes_.push_back(plane);

	//--------------------------------------------------Sphere
	Sphere* sphere	= new Sphere(Vec3(0, -1.5, -5), .5);
	sphere->material_ = new MATERIAL;
	shapes_.push_back(sphere);

	sphere			  = new Sphere(Vec3(-1, -1.5, -5), .5);
	sphere->material_ = new MATERIAL;
	shapes_.push_back(sphere);

	sphere			  = new Sphere(Vec3(1, -1.5, -5), .5);
	sphere->material_ = new MATERIAL;
	shapes_.push_back(sphere);

	sphere			  = new Sphere(Vec3(0, -1.5, -4), .5);
	sphere->material_ = new MATERIAL;
	shapes_.push_back(sphere);
}

//--------------------------------------------------Y ���ϣ� ����
void World::buildScene2()
{
	Plane* plane = nullptr;

	plane			 = new Plane(Vec3(0, -25, 0), Vec3(0, 1, 0));
	plane->material_ = new Checker();
	shapes_ << plane;

	int i = 0;

	static Mesh bunyMesh("../3rd/bunny.obj");

	vector<float> xVec, zVec;
	g::RANDOM_PT(xVec, zVec);

	Matte* matte				   = new Matte;
	matte->diffuse_brdf.diffuse_.k = .95;

	//-------------------------------------------------bunny
	for (i = 0; i < 1; i++)
	{
		MeshObject* bunny = new MeshObject(bunyMesh);

		bunny->matrix_   = /*Matrix::rotate(0, Vec3(0, 1, 0)) **/ Matrix::scale(10) * Matrix::translate(0, -3, -10);
		bunny->material_ = matte;
		shapes_ << bunny;
	}

	//-------------------------------------------------venusm
	static Mesh venusmMesh("../3rd/venusm.obj");
	MeshObject* venusm = new MeshObject(venusmMesh);

	venusm->matrix_   = Matrix::rotate(0, Vec3(0, 1, 0)) * Matrix::scale(.01) * Matrix::translate(30, -10, -50);
	venusm->material_ = matte;
	//shapes_ << venusm;

	//-------------------------------------------------ateneam
	static Mesh ateneammMesh("../3rd/ateneam.obj");
	MeshObject* ateneam = new MeshObject(ateneammMesh);

	ateneam->matrix_   = Matrix::rotate(0, Vec3(0, 1, 0)) * Matrix::scale(.01) * Matrix::translate(-30, -5, -100);
	ateneam->material_ = matte;
	//shapes_ << ateneam;

	//-------------------------------------------------Sphere
	//Sphere* sphere = new Sphere(Vec3(0, -18, -100), 8);
	//sphere->material_ = matte;
	//shapes_ << sphere;

	PointLight* pLight = new PointLight;
	pLight->pt_		   = Vec3(0, 10, -10);
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

//����������ɫ
Vec3 World::trace_ray(const Ray ray, int depth)
{
	ShadeInfo info(this->intersection(ray));

	if (info.valid())
	{
		info.depth = depth;
		info.ray   = ray;

		Vec3 c = info.material->shade(info);
		return c;
	}

	return bgColor_;
}

Vec3 World::trace_ray_direct(const Ray ray, int depth)
{
	ShadeInfo info(this->intersection(ray));

	if (info.valid())
	{
		info.depth = depth;
		info.ray   = ray;

		Vec3 c = info.material->getColor(info);

		if (isInShadow(info))
			c *= .1;

		return c;
	}

	return bgColor_;
}

void World::max_to_one(Vec3& c) const
{
	if (c.r() > 1 || c.g() > 1 || c.b() > 1)
	{
		float maxV = std::max({ kEpsilon, c.r(), c.g(), c.b() });

		c /= maxV;
	}
}

void World::clamp_to_color(Vec3& c, const Vec3& clamto /*= g::Red*/) const
{
	if (c.r() > 1 || c.g() > 1 || c.b() > 1)
	{
		c = clamto;
	}
}

Vec3 World::get_ambient() const
{
	return Vec3(.05);
}

bool World::isInShadow(ShadeInfo& info) const
{
	return false;
	for (PointLight* pl : Light::get_type<PointLight>())
	{
		Ray shadowRay;
		shadowRay.orig = info.position;
		shadowRay.dir = pl->pt_ - info.position;
		shadowRay.dir.normalize();

		ShadeInfo shadowinfo;// = World::Instance()->intersection(shadowRay);

		//shadowinfo = accel_->intersect(shadowRay);
		//ShadeInfo tmpInfo = intersection_without_meshobject(shadowRay);

		//if (tmpInfo.valid() && tmpInfo.dis < shadowinfo.dis)
		//{
		//	shadowinfo = tmpInfo;
		//}

		shadowinfo = Shape::get_type<MeshObject>()[0]->intersect(shadowRay);
			
		if (!shadowinfo.valid())
		{
			return false;
		}
		else
		{
			//cout << shadowinfo.shape->className() << "\t" << cc << "\n";
		}
	}

	for (DirectionLight* pl : Light::get_type<DirectionLight>())
	{
		if (info.normal * pl->dir_ > kEpsilon) //˵���ܱ��������䵽
			return false;
	}

	return true;
}
