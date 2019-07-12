#include "stdafx.h"
#include "Plane.h"

Plane::Plane()
{
}

Plane::Plane(const Vec3& position, const Vec3& normal)
	: _position(position), _normal(normal)
{
}

Plane::~Plane()
{
}

ShadeInfo Plane::intersect(const Ray& ray)
{
	ShadeInfo info;

	float t = (_position - ray.orig) * _normal / (ray.dir * _normal);

	if (t > 1e-4)
	{
		info.normal   = _normal;
		info.hit_pos = ray.orig + ray.dir * t;
		info.dis	  = t;
		info.material = this->material_;
		info.shape	= this;
		info.ray	  = ray;

		if (x_min != x_max)
		{
			if (info.hit_pos.x() < x_min || info.hit_pos.x() > x_max)
				return ShadeInfo();
		}

		if (y_min != y_max)
		{
			if (info.hit_pos.y() < y_min || info.hit_pos.y() > y_max)
				return ShadeInfo();
		}

		if (z_min != z_max)
		{
			if (info.hit_pos.z() < z_min || info.hit_pos.z() > z_max)
				return ShadeInfo();
		}
	}

	return info;
}
