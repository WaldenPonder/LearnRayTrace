#include "stdafx.h"
#include "Sphere.h"

Sphere::Sphere()
{

}


Sphere::Sphere(const Vec3& center, float radius) : _center(center), _radius(radius)
{
}


Sphere::~Sphere()
{
}

ShadeInfo Sphere::intersect(const Ray& ray)
{
	ShadeInfo info;

	Vec3 v      = ray.orig - _center;
	float a0    = v.length2() - _radius * _radius;
	float DdotV = ray.dir * v;

	if (DdotV <= 0)
	{
		float discr = DdotV * DdotV - a0;

		if (discr >= 0)
		{
			info.setShape(this);
			info.dis = -DdotV - sqrt(discr);
			info.position = ray.distance(info.dis);
			info.normal   = info.position - _center;
			info.normal.normalize();
			info.ray      = ray;
		}
	}

	return info;
}
