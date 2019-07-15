#pragma once
#include "Matte.h"
#include "GlossySpecular.h"

class Phong : public Matte
{
public:
	__class__(Phong);
	Phong(const Vec3& cd, float kd, const Vec3& cs, float ks, float exp);
	virtual ~Phong();

	virtual Vec3 shade(ShadeInfo& si) override;
	virtual Vec3 shade_direct(ShadeInfo& si) override;

private:
	GlossySpecular glossySpecular_;
};

