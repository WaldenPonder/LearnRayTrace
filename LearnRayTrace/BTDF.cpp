#include "stdafx.h"
#include "BTDF.h"


BTDF::BTDF()
{
}


BTDF::~BTDF()
{
}

Vec3 BTDF::f(const ShadeInfo& si, const Vec3& wo, const Vec3& wi) const
{
	return Vec3();
}

Vec3 BTDF::sample_f(const ShadeInfo& si, const Vec3& wo, Vec3& wt) const
{
	return Vec3();
}

bool BTDF::tir(const ShadeInfo& si) const
{
	return false;
}
