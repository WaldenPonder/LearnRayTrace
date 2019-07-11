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
	Vec3 wi = -info.ray.dir + 2 * info.normal * info.ray.dir * info.normal;

	Vec3 fr = rc_.get() / abs(wi * info.normal);

	Ray reflect_ray(info.position, wi);
	L += componentMultiply(fr, World::Instance()->trace_ray_direct(reflect_ray, info.depth + 1));

	return L;
}