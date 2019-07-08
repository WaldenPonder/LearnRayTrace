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
	buildScene1();
}

World::~World() {}

//--------------------------------------------------Y 向上， 右手
void World::buildScene1()
{
	Matte* matte_red = new Matte;
	matte_red->diffuse_brdf.diffuse_.c = g::Red;
	matte_red->diffuse_brdf.diffuse_.k = .95;

	Matte* matte_sky = new Matte;
	matte_sky->diffuse_brdf.diffuse_.c = g::SummerSky;
	matte_sky->diffuse_brdf.diffuse_.k = .95;

	Matte* matte_blue = new Matte;
	matte_blue->diffuse_brdf.diffuse_.c = g::Blue;
	matte_blue->diffuse_brdf.diffuse_.k = .95;

	Matte* matte = new Matte;
	matte->diffuse_brdf.diffuse_.k = .95;

	Plane* plane = nullptr;

	PointLight* pLight = new PointLight;
	pLight->pt_ = Vec3(0, 0, 0);

	plane			 = new Plane(Vec3(0, 2, 0), Vec3(0, -1, 0));  //上
	plane->material_ = matte_blue;

	plane			 = new Plane(Vec3(0, 2 - 1e-5, 0), Vec3(0, -1, 0));  //灯
	plane->material_ = new Emissive;
	plane->x_min = -.5, plane->x_max = .5;
	plane->z_min = -5, plane->z_max = -4;

	plane			 = new Plane(Vec3(0, -2, 0), Vec3(0, 1, 0));  //下
	plane->material_ = matte_blue;

	plane			 = new Plane(Vec3(-2, 0, 0), Vec3(1, 0, 0));  //左
	plane->material_ = matte_sky;

	plane			 = new Plane(Vec3(0, 0, -6), Vec3(0, 0, 1));  //后
	plane->material_ = matte_red;

	plane			 = new Plane(Vec3(2, 0, 0), Vec3(-1, 0, 0));  //右
	plane->material_ = matte_sky;

	//--------------------------------------------------Sphere
	//Sphere* sphere	= new Sphere(Vec3(0, -1.5, -5), .5);
	//sphere->material_ = matte_sky;

	//sphere			  = new Sphere(Vec3(-1, -1.5, -5), .5);
	//sphere->material_ = matte_red;

	//sphere			  = new Sphere(Vec3(1, -1.5, -5), .5);
	//sphere->material_ = matte_sky;

	//sphere			  = new Sphere(Vec3(0, -1.5, -4), .5);
	//sphere->material_ = matte_sky;
	
	//-------------------------------------------------bunny
	static Mesh bunyMesh("../3rd/bunny.obj");
	MeshObject* bunny = new MeshObject(bunyMesh);
	bunny->matrix_ = Matrix::rotate(0, Vec3(0, 1, 0)) * Matrix::scale(10) * Matrix::translate(0, 0, -3);
	bunny->material_ = matte_sky;
}

//--------------------------------------------------Y 向上， 右手
void World::buildScene2()
{
	Plane* plane = nullptr;

	//plane			 = new Plane(Vec3(0, -25, 0), Vec3(0, 1, 0));
	//plane->material_ = new Checker();

	int i = 0;

	static Mesh bunyMesh("../3rd/bunny_big.obj");

	vector<float> xVec, zVec;
	g::RANDOM_PT(xVec, zVec);

	Matte* matte				   = new Matte;
	matte->diffuse_brdf.diffuse_.k = .95;

	//-------------------------------------------------bunny
	for (i = 0; i < 1; i++)
	{
		MeshObject* bunny = new MeshObject(bunyMesh);

		bunny->matrix_   = Matrix::rotate(0, Vec3(0, 1, 0)) * Matrix::scale(10) * Matrix::translate(0, 0, -5);
		bunny->material_ = matte;
		const BoundingBox& bb = bunny->computBBox(); 
		//cout << bb.max_len() << "\t" << bb.min_len() << "\tXX";
	}

	//-------------------------------------------------venusm
	static Mesh venusmMesh("../3rd/venusm.obj");
	//MeshObject* venusm = new MeshObject(venusmMesh);

	//venusm->matrix_   = Matrix::rotate(0, Vec3(0, 1, 0)) * Matrix::scale(.01) * Matrix::translate(30, -10, -50);
	//venusm->material_ = matte;

	//-------------------------------------------------ateneam
	static Mesh ateneammMesh("../3rd/ateneam.obj");
	//MeshObject* ateneam = new MeshObject(ateneammMesh);
	//ateneam->matrix_   = Matrix::rotate(0, Vec3(0, 1, 0)) * Matrix::scale(.01) * Matrix::translate(-30, -5, -100);
	//ateneam->material_ = matte;

	//-------------------------------------------------Sphere
	//Sphere* sphere = new Sphere(Vec3(-30, -17, -50), 8);
	//sphere->material_ = matte;

	PointLight* pLight = new PointLight;
	pLight->pt_		   = Vec3(10, 100, 10);
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

	for (Shape* s : Shape::pool())
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

	for (Shape* s : Shape::pool())
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
	for (PointLight* pl : Light::get_type<PointLight>())
	{
		Ray shadowRay;
		shadowRay.dir = pl->pt_ - info.position;
		shadowRay.dir.normalize();
		shadowRay.orig = info.position + shadowRay.dir * .001;

		ShadeInfo shadowinfo = accel_->intersect(shadowRay);
		ShadeInfo tmpInfo = intersection_without_meshobject(shadowRay);

		if (tmpInfo.valid() && tmpInfo.dis < shadowinfo.dis)
		{
			shadowinfo = tmpInfo;
		}

		//shadowinfo = Shape::get_type<MeshObject>()[0]->intersect(shadowRay);		
		if (!shadowinfo.valid() || shadowinfo.dis > (info.position - pl->pt_).length())
		{
			return false;
		}
		else
		{
			//cout << shadowinfo.dis << "\t" << info.shape->className() << "\t" << shadowinfo.shape->className() << "\t" << cc << "\n";
		}
	}

	for (DirectionLight* pl : Light::get_type<DirectionLight>())
	{
		if (info.normal * pl->dir_ > 0) //说明能被光线照射到
			return false;
	}

	return true;
}
