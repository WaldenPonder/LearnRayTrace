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
		info.normal = _normal;
		info.position = ray.orig + ray.dir * t;
		info.distance = t;
		info.material = this->_material;
		info.shape = this;
		info.ray = ray;

		if (x_min != x_max)
		{
			if (info.position.x() < x_min || info.position.x() > x_max)
				return ShadeInfo();
		}

		if (y_min != y_max)
		{
			if (info.position.y() < y_min || info.position.y() > y_max)
				return ShadeInfo();
		}

		if (z_min != z_max)
		{
			if (info.position.z() < z_min || info.position.z() > z_max)
				return ShadeInfo();
		}
	}


   return info;
}
