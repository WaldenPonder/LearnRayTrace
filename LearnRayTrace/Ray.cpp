#include "stdafx.h"
#include "Ray.h"


Ray::Ray()
{
}


Ray::Ray(const Vec3& o, const Vec3& d) : orig(o), dir(d)
{

}

Ray::~Ray()
{
}

Point Ray::distance(float t) const
{
	return orig + dir * t;
}
