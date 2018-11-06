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

#if 0
   float denom = ray.dir * _normal;

   if (denom > 1e-6)
   {
	   Vec3 p0l0 = ray.orig - _position;
	   p0l0.normalize();

	   float t = (p0l0 * _normal) / denom;

	   if (t >= 0)
	   {
		   result.shape = this;
		   result.normal = _normal;
	   }
   }
#else

	float t = (_position - ray.orig) * _normal / (ray.dir * _normal);

	if (t > 1e-4)
	{
		info.normal = _normal;
		info.position = ray.orig + ray.dir * t;
		info.distance = t;
		info.material = this->_material;
		info.shape = this;
		info.ray = ray;
	}
	   
#endif

   return info;
}
