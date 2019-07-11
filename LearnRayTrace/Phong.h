#pragma once
#include "Matte.h"
#include "GlossySpecular.h"

class Phong : public Matte
{
public:
	Phong(const Vec3& cd, float kd, const Vec3& cs, float ks, float exp);
	virtual ~Phong();

	virtual Vec3 shade(ShadeInfo& r) override;
	virtual Vec3 shade_direct(ShadeInfo& r) override;

private:
	GlossySpecular glossySpecular_;
};

