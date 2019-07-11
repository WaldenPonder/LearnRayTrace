#include "stdafx.h"
#include "Checker.h"
#include "World.h"


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

Vec3 Checker::shade_direct(ShadeInfo& info)
{
	static float factor = .05;
	Vec3 c = fabs((int)(floor(info.position.x() * factor) + floor(info.position.z() * factor)) % 2) < 1 ? g::SummerSky : g::White;

	if (World::Instance()->isInShadow(info))
		c *= .1;
	return c;
}
