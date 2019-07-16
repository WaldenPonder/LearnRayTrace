#include "stdafx.h"
#include "Transparent.h"
#include "PerfectTransmitter.h"
#include "PerfectSpecular.h"
#include "World.h"
#include <assert.h>

struct Transparent::Impl
{
	Impl(){}
	~Impl() { delete specular_btdf; delete reflective_brdf; }
	PerfectTransmitter* specular_btdf; //ÕÛÉä
	PerfectSpecular* reflective_brdf; //·´Éä
};

Transparent::Transparent(const Vec3& cd, float kd, const Vec3& cs, float ks, float exp) : Phong(cd, kd, cs, ks, exp)
{
	impl = new Impl;
	impl->reflective_brdf = new PerfectSpecular;
	impl->specular_btdf = new PerfectTransmitter;
}


Transparent::~Transparent()
{
	delete impl;
}

Vec3 Transparent::shade_direct(ShadeInfo& si)
{
	Vec3 L(Phong::shade_direct(si));

	Vec3 wo = -si.ray.dir;
	Vec3 wi;

	Vec3 fr = impl->reflective_brdf->sample_f(si, wo, wi);

	Ray reflected_ray(si.hit_pos, wi);

	if (impl->specular_btdf->tir(si))
	{
		L += World::Instance()->trace_ray_direct(reflected_ray, si.depth + 1);
	}
	else
	{
		Vec3 wt;
		Vec3 ft = impl->specular_btdf->sample_f(si, wo, wt);

		Ray transmitted_ray(si.hit_pos, wt);

		Vec3 rc = World::Instance()->trace_ray_direct(reflected_ray, si.depth + 1) * fabs(si.normal * wi);
		Vec3 tc = World::Instance()->trace_ray_direct(transmitted_ray, si.depth + 1) *fabs(si.normal * wt);

		L += componentMultiply(fr, rc);
		L += componentMultiply(ft, tc);
	}

	return L;
}

void Transparent::set_refract(float kt, float ior)
{
	impl->specular_btdf->kt_ = kt;
	impl->specular_btdf->ior_ = ior;
}

void Transparent::set_reflect(const Color& rc)
{
	impl->reflective_brdf->rc_ = rc;
}
