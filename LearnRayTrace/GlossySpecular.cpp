#include "stdafx.h"
#include "GlossySpecular.h"
#include "Material.h"
#include "MultiJittered.h"
#include <cmath>

GlossySpecular::GlossySpecular(const Color& c, float exp) : glossyColor_(c), exp_(exp)
{
	sampler_ = MultiJittered::instance();
}

GlossySpecular::~GlossySpecular() {}

Vec3 GlossySpecular::f(const ShadeInfo& info, const Vec3& wo, const Vec3& wi) const
{
	float ndotwi = info.normal * wi;
	Vec3  r = -wi + info.normal * ndotwi * 2.0;  // direction of mirror reflection

	float phong_lobe = pow(r * wo, exp_);
	
	return (glossyColor_.get() * phong_lobe);
}

Vec3 GlossySpecular::sample_f(const ShadeInfo& info, const Vec3& wo, Vec3& wi, float& pdf) const
{
	float ndotwo = info.normal * wo;
	Vec3  r		 = -wo + info.normal * ndotwo * 2.0;  // direction of mirror reflection

	Vec3 w = r;
	Vec3 u = Vec3(0.00424, 1, 0.00764) ^ w;
	u.normalize();
	Vec3 v = u ^ w;

	Point sp = sampler_->sample_hemisphere();
	wi		 = sp.x() * u + sp.y() * v + sp.z() * w;  // reflected ray direction

	if (info.normal * wi < 0.0)  // reflected ray is below tangent plane
		wi = -sp.x() * u - sp.y() * v + sp.z() * w;

	float phong_lobe = pow(r * wi, exp_);
	pdf				 = phong_lobe * (info.normal * wi);

	return (glossyColor_.get() * phong_lobe);
}
