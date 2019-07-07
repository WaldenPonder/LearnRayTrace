#include "stdafx.h"
#include "BRDF.h"


BRDF::BRDF()
{
}


BRDF::~BRDF()
{
}

Vec3 BRDF::f(const ShadeInfo& si, const Vec3& wo, Vec3& wi) const
{
	return Vec3();
}

Vec3 BRDF::sample_f(const ShadeInfo& si, const Vec3& wo, Vec3& wi, float& pdf) const
{
	return g::Black;
}
