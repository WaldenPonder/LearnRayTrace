#include "stdafx.h"
#include "World.h"
#include "Plane.h"
#include "Checker.h"
#include "Sphere.h"
#include "Emissive.h"
#include "Matte.h"
#include "MeshObject.h"
#include "Acceleration.h"
#include "Phong.h"

#undef max
#undef min
#include <random>
#include "Light.h"
#include <assert.h>

#define MATERIAL Matte
#define SCENE_2

#include "WorldImpl.inc"

World::World()
{
	max_depth_ = 2;
	buildScene();
}

World::~World() {}

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

//·µ»Ø×îÖÕÑÕÉ«
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

		Vec3 c = info.material->shade_direct(info);

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
	if (info.isInShadow == ShadeInfo::YES)
		return true;
	else if (info.isInShadow == ShadeInfo::NO)
		return false;

	Ray shadowRay;

	shadowRay.orig = info.position;

	ShadeInfo shadowinfo;

	for (PointLight* pl : Light::get_type<PointLight>())
	{
		shadowRay.dir = pl->pt_ - info.position;
		shadowRay.dir.normalize();
		shadowRay.calcuPreValue();

		//if(accel_) shadowinfo = accel_->intersect(shadowRay);

		for (Shape* s : Shape::pool())
		{
			if (s == info.shape)
				continue;

			ShadeInfo value = s->intersect(shadowRay);
			if (value.valid())
			{
				if (value.dis < shadowinfo.dis)
				{
					shadowinfo = value;
				}
			}
		}
		
		//shadowinfo = Shape::get_type<MeshObject>()[0]->intersect(shadowRay);		
		if(shadowinfo.valid() && shadowinfo.dis < (info.position - pl->pt_).length())
		{
			info.isInShadow = ShadeInfo::YES;
			return true;			
		}
	}

	for (DirectionLight* pl : Light::get_type<DirectionLight>())
	{
		shadowRay.dir = pl->getDir(info);
		shadowRay.dir.normalize();
		shadowRay.calcuPreValue();

		for (Shape* s : Shape::pool())
		{
			if (s == info.shape)
				continue;

			ShadeInfo value = s->intersect(shadowRay);
			if (value.valid())
			{
				info.isInShadow = ShadeInfo::YES;
				//cout << "a\n";
				return true;
			}
		}
	}
	info.isInShadow = ShadeInfo::NO;

	return false;
}
