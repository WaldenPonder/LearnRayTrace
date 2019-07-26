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

Vec3 Transparent::shade(ShadeInfo& si)
{
	Vec3 L(Matte::lambert_f());

	Vec3 wo = -si.ray.dir;
	Vec3 wi;
	Vec3 fr = impl->reflective_brdf->sample_f(si, wo, wi);
	Ray reflected_ray(si.hit_pos, wi);

	if (impl->specular_btdf->tir(si))
	{
		L += World::Instance()->trace_ray(reflected_ray, si.depth + 1);
	}
	else
	{
		const float FN = fresnel(si);
		impl->reflective_brdf->rc_.k = FN;
		impl->specular_btdf->kt_ = 1 - FN;

		Vec3 wt;
		Vec3 ft = impl->specular_btdf->sample_f(si, wo, wt);

		Ray transmitted_ray(si.hit_pos, wt);

		Vec3 rc = World::Instance()->trace_ray(reflected_ray, si.depth + 1) * fabs(si.normal * wi);
		Vec3 tc = World::Instance()->trace_ray(transmitted_ray, si.depth + 1) *fabs(si.normal * wt);

		L += componentMultiply(fr, rc);
		L += componentMultiply(ft, tc);
	}

	return L;
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
		const float FN = fresnel(si);
		impl->reflective_brdf->rc_.k = FN;
		impl->specular_btdf->kt_ = 1 - FN;
		
		Vec3 wt;
		Vec3 ft = impl->specular_btdf->sample_f(si, wo, wt);

		Ray transmitted_ray(si.hit_pos, wt);

		Vec3 rc = World::Instance()->trace_ray_direct(reflected_ray, si.depth + 1) * fabs(si.normal * wi);
		Vec3 tc = World::Instance()->trace_ray_direct(transmitted_ray, si.depth + 1) *fabs(si.normal * wt);

		//cout << componentMultiply(ft, tc) << "\n";
		L += componentMultiply(fr, rc);
		L += componentMultiply(ft, tc);
	}

	return L;
}

void Transparent::set_refract(float ior)
{
	impl->specular_btdf->ior_ = ior;
}

void Transparent::set_reflect_color(const Vec3& rc)
{
	impl->reflective_brdf->rc_.c = rc;
}

float Transparent::fresnel(const ShadeInfo& si) const
{
	float eta_out = 1.f;
	float eta_in = impl->specular_btdf->ior_;

	Vec3 normal(si.normal);
	Vec3 wo = -si.ray.dir;
	float ndotd = normal * wo;
	float eta = eta_in / eta_out;

	if (ndotd < 0.0)
	{
		normal = -normal;
		eta = 1 / eta;
	}

	float cos_theta_i = normal * wo;

	float temp = 1.0 - (1.0 - cos_theta_i * cos_theta_i) / (eta * eta);
	float cos_theta_t = std::sqrt(temp);

	float r_parallel = (eta * cos_theta_i - cos_theta_t) / (eta * cos_theta_i + cos_theta_t);
	float r_perpendicular = (cos_theta_i - eta * cos_theta_t) / (cos_theta_i + eta * cos_theta_t);

	float kr = 0.5 * (r_parallel * r_parallel + r_perpendicular * r_perpendicular);

	return kr;
}
