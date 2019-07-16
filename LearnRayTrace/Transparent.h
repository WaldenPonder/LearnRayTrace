#pragma once
#include "Phong.h"

class Transparent : public Phong
{
public:
	__class__(Transparent);

	Transparent(const Vec3& cd, float kd, const Vec3& cs, float ks, float exp);
	~Transparent();

	virtual Vec3 shade_direct(ShadeInfo& si) override;

	void set_refract(float ior);
	void set_reflect_color(const Vec3& rc);

	float fresnel(const ShadeInfo& si) const;

	struct Impl;
	Impl* impl;
};

