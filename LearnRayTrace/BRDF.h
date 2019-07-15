#pragma once
#include "Vec3.h"
#include "ShadeInfo.h"

class Sampler;

class BRDF
{
public:
	BRDF();
	~BRDF();

	virtual Vec3 f(const ShadeInfo& si, const Vec3& wo, const Vec3& wi) const;
	virtual Vec3 sample_f(const ShadeInfo& si, const Vec3& wo, Vec3& wi, float& pdf) const;

	Sampler* sampler_ = nullptr;
};

