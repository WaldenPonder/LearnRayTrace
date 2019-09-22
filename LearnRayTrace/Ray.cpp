#include "stdafx.h"
#include "Ray.h"

Ray::Ray()
{

}

Ray::Ray(const Vec3& o, const Vec3& d, int dp) : orig(o), dir(d), depth(dp)
{
	calcuPreValue();
}

Ray::~Ray()
{
}

Point Ray::distance(float t) const
{
	return orig + dir * t;
}

void Ray::calcuPreValue()
{
	for (uint8_t i = 0; i < 7; ++i)
	{
		precomputedNumerator[i] = g::planeSetNormals[i] * orig;
		precomputedDenominator[i] = g::planeSetNormals[i] * dir;
	}
}
