#include "stdafx.h"
#include "Checker.h"
#include "World.h"


Checker::Checker(const Vec3& c1 /*= g::Black*/, const Vec3& c2 /*= g::White*/, float kr /*= .75*/)
{
	c1_ = c1, c2_ = c2, kr_ = kr;
}

Checker::~Checker()
{
}

Vec3 Checker::shade(ShadeInfo& r)
{
	return shade_direct(r);
}

Vec3 Checker::shade_direct(ShadeInfo& si)
{
	static float factor = .05f;
	
	//-------------------------------------------------反射部分	
	Vec3 wi = si.reflect();

	Vec3 fr(kr_);

	Ray reflect_ray(si.hit_pos, wi, si.ray.depth + 1);
	Vec3 cr = componentMultiply(fr, World::Instance()->trace_ray_direct(reflect_ray));

	Vec3 c = fabs((int)(floor(si.hit_pos.x() * factor) + floor(si.hit_pos.z() * factor)) % 2) < 1 ? c1_ : c2_;
	
	//if(!cr.isNearZero()) //如果有反射则显示反射的颜色
	//	c = cr;

	//if (World::Instance()->isInShadow(si))
	//	c *= .1;

	return c;
}
