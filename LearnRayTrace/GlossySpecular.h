#pragma once
#include "BRDF.h"

class GlossySpecular : public BRDF
{
public:
	GlossySpecular();
	~GlossySpecular();

	virtual Color sample_f(const ShadeInfo& si, const Vec3& wo, Vec3& wi, float& pdf) const;
};

