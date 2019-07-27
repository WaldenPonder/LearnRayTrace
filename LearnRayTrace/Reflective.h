#pragma once
#include "Phong.h"

class PerfectSpecular;
class Reflective : public Phong
{
public:
	__class__(Reflective);
	Reflective(const Vec3& cd, float kd, const Vec3& cs, float ks, float exp);
	~Reflective();

	void set_reflect(const Color& rc);

	virtual Vec3 shade_direct(ShadeInfo& si) override;
	virtual Vec3 shade(ShadeInfo& si) override;

private:
	PerfectSpecular* perfectSpecular_;
};

