#pragma once
#include "BRDF.h"

class PerfectSpecular : public BRDF
{
public:
	PerfectSpecular();
	virtual ~PerfectSpecular();

	virtual Vec3 sample_f(const ShadeInfo& si, const Vec3& wo, Vec3& wi) const;

	Color rc_;
};

