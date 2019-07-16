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
		float t = -DdotV - sqrt(discr);

		if (discr >= 0 && t > .0001)
		{
			info.setShape(this);
			info.dis = t;
			info.hit_pos = ray.distance(info.dis);
			info.normal   = info.hit_pos - _center;
			info.normal.normalize();
			info.ray      = ray;
		}
	}

	return info;
}
