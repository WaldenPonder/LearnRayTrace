#include "stdafx.h"
#include "Ray.h"


Ray::Ray()
{
}


Ray::Ray(const Vec3& o, const Vec3& d) : orig(o), dir(d)
{
	for (uint8_t i = 0; i < 7; ++i)
	{
		precomputedNumerator[i] = g::planeSetNormals[i] * orig;
		precomputedDenominator[i] = g::planeSetNormals[i] * dir;
	}
}

Ray::~Ray()
{
}

Point Ray::distance(float t) const
{
	return orig + dir * t;
}
