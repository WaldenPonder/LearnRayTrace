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
#include "Reflective.h"
#include "Transparent.h"

#undef max
#undef min
#include <random>
#include "Light.h"
#include <assert.h>
#include "WorldImpl.inc"
#include "PhotonMap.h"
#include "MultiJittered.h"

static const bool USE_SHDOW = false;

World::World()
{
	max_depth_ = 2;
	buildScene();
}

World::~World() {}

void World::build_photon_map(const int photon_num)
{
	is_collect_photon_	= true;
	const int max_threads = std::thread::hardware_concurrency();

	const float power = .7;
	const Vec3  photon_power(power / photon_num, power / photon_num, power / photon_num);

	auto FUNC = [photon_num, max_threads, photon_power, this](int thread_index) {
		for (int i = 0; i < photon_num; i++)
		{
			if (i % max_threads != thread_index)
				continue;

			float x = rand_float(-.5, .5);
			float y = rand_float(1.85, 2.01);
			float z = rand_float(-5, -4);

			Vec3 w(0, -1, 0);
			Vec3 u = Vec3(0.00424, 1, 0.00764) ^ w;
			u.normalize();
			Vec3 v = u ^ w;
			//Vec3 sp = MultiJittered::instance()->sample_hemisphere();
			Vec3 sp  = g::random_cosine_direction();
			Vec3 dir = sp.x() * u + sp.y() * v + sp.z() * w;
			dir.normalize();

			Ray ray(Vec3(0, 1.85, -4.5), dir);
			collect_photon(ray, 0, photon_power);
		}
	};

	vector<std::shared_ptr<std::thread>> ths;

	for (int i = 0; i < max_threads; i++)
	{
		std::shared_ptr<std::thread> th = std::make_shared<std::thread>(FUNC, i);
		ths.push_back(th);
	}

	for (std::shared_ptr<std::thread>& th : ths)
	{
		th->join();
	}

	is_collect_photon_ = false;
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
		if (is_collect_photon_ && s->className() == IBox::class_name())  //创建photon map时，不考虑跟光源相交
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

Vec3 World::trace_ray_direct(const Ray ray)
{
	ShadeInfo info(this->intersection(ray));

	if (info.valid())
	{
		info.ray = ray;
		Vec3 c   = info.material->shade_direct(info);
		return c;
	}

	return bgColor_;
}

Vec3 World::trace_photon(const Ray ray)
{
	ShadeInfo info(this->intersection(ray));

	if (info.valid())
	{
		if (info.shape->className() == IBox::class_name())
		{
			return info.material->shade(info);
		}
		else
		{
			float		   max_distance = 0;
			vector<Photon> photons		= PhotonMap::Instance()->find_nearst<50>(info.hit_pos, max_distance);

			Vec3 c;

			for (auto& p : photons)
			{
				c += p.color;
			}

			//cout << "max_distance\t" << max_distance << endl;
			c /= (PI * max_distance * max_distance);
			return c;
		}
	}

	return bgColor_;
}

void World::collect_photon(const Ray ray, int depth, Vec3 c)
{
	if (depth > max_depth_) return;

	ShadeInfo info(this->intersection(ray));

	if (info.valid())
	{
		info.depth = depth;
		info.ray   = ray;
		info.material->collect_photon(info, c);
	}
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
	if (!USE_SHDOW) return false;

	if (info.isInShadow == ShadeInfo::YES)
		return true;
	else if (info.isInShadow == ShadeInfo::NO)
		return false;

	Ray shadowRay;

	shadowRay.orig = info.hit_pos;

	ShadeInfo shadowinfo;

	for (PointLight* pl : Light::get_type<PointLight>())
	{
		shadowRay.dir = pl->pt_ - info.hit_pos;
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
		if (shadowinfo.valid() && shadowinfo.dis < (info.hit_pos - pl->pt_).length())
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
