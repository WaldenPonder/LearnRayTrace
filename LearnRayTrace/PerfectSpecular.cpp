#include "stdafx.h"
#include "PerfectSpecular.h"


PerfectSpecular::PerfectSpecular()
{
}


PerfectSpecular::~PerfectSpecular()
{
}

Vec3 PerfectSpecular::sample_f(const ShadeInfo& si, const Vec3& wo, Vec3& wi) const
{
	wi = si.reflect();
	return (rc_.get() / fabs(si.normal * wi));
}
