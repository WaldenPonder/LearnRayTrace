#include "stdafx.h"
#include "BRDF.h"


BRDF::BRDF()
{
}


BRDF::~BRDF()
{
}

Color BRDF::sample_f(const ShadeInfo& si, const Vec3& wo, Vec3& wi, float& pdf) const
{
	return g::Black;
}
