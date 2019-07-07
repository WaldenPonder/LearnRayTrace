#pragma once
#include "BRDF.h"

class GlossySpecular : public BRDF
{
public:
	GlossySpecular();
	~GlossySpecular();

	virtual Vec3 sample_f(const ShadeInfo& si, const Vec3& wo, Vec3& wi, float& pdf) const;

	Color glossyColor_;
};

