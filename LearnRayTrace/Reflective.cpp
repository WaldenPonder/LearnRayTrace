#include "stdafx.h"
#include "Reflective.h"
#include <cmath>
#include "BRDF.h"
#include "MultiJittered.h"
#include "World.h"
#include "PerfectSpecular.h"

Reflective::Reflective(const Vec3& cd, float kd, const Vec3& cs, float ks, float exp) : Phong(cd, kd, cs, ks, exp)
{
	perfectSpecular_ = new PerfectSpecular;
}

Reflective::~Reflective()
{
}

void Reflective::set_reflect(const Color& rc)
{
	perfectSpecular_->rc_ = rc;
}

Vec3 Reflective::shade_direct(ShadeInfo& si)
{
	Vec3 L(Phong::shade_direct(si));
	Vec3 wi;
	Vec3 f = perfectSpecular_->sample_f(si, -si.ray.dir, wi);

	Ray reflect_ray(si.hit_pos, wi);
	Vec3 c = componentMultiply(f, World::Instance()->trace_ray_direct(reflect_ray, si.depth + 1));
	L += c;
	return L.clamp(0, 1);
}