#include "stdafx.h"
#include "Reflective.h"
#include <cmath>
#include "BRDF.h"
#include "MultiJittered.h"
#include "World.h"

Reflective::Reflective(const Vec3& cd, float kd, const Vec3& cs, float ks, float exp) : Phong(cd, kd, cs, ks, exp)
{
}

Reflective::~Reflective()
{
}

void Reflective::set_rc(const Color& rc)
{
	rc_ = rc;
}

Vec3 Reflective::shade_direct(ShadeInfo& info)
{
	Vec3 L(Phong::shade_direct(info));

	Vec3 wi = info.reflect();
	Vec3 fr = rc_.get();

	Ray reflect_ray(info.hit_pos, wi);
	Vec3 c = componentMultiply(fr, World::Instance()->trace_ray_direct(reflect_ray, info.depth + 1));
	L += c;
	return L.clamp(0, 1);
}