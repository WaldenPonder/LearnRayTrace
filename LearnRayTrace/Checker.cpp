#include "stdafx.h"
#include "Checker.h"


Checker::Checker()
{
}


Checker::~Checker()
{
}

Vec3 Checker::shade(ShadeInfo& r)
{
	return shade_direct(r);
}

Vec3 Checker::shade_direct(ShadeInfo& r)
{
	static float factor = .05;
	return fabs((int)(floor(r.position.x() * factor) + floor(r.position.z() * factor)) % 2) < 1 ? g::SummerSky : g::White;
}
