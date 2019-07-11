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

Vec3 Checker::shade_direct(ShadeInfo& info)
{
	static float factor = .05;
	
	//-------------------------------------------------·´Éä²¿·Ö
	Vec3 wo = -info.ray.dir;
	Vec3 wi = -wo + 2. * info.normal * wo * info.normal;

	Vec3 fr(kr_);

	Ray reflect_ray(info.position, wi);
	Vec3 cr = componentMultiply(fr, World::Instance()->trace_ray_direct(reflect_ray, info.depth + 1));

	Vec3 c = fabs((int)(floor(info.position.x() * factor) + floor(info.position.z() * factor)) % 2) < 1 ? c1_ : c2_;
	
	c += cr;

	if (World::Instance()->isInShadow(info))
		c *= .1;

	World::Instance()->max_to_one(c);
	return c;
}
