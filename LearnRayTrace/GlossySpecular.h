#pragma once
#include "BRDF.h"

class GlossySpecular : public BRDF
{
public:
	GlossySpecular(const Color& c, float exp);
	~GlossySpecular();

	virtual Vec3 f(const ShadeInfo& si, const Vec3& wo, const Vec3& wi) const override;
	virtual Vec3 sample_f(const ShadeInfo& si, const Vec3& wo, Vec3& wi, float& pdf) const;

private:
	Color glossyColor_;
	float exp_;
};

