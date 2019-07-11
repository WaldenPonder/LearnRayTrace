#pragma once
#include "Phong.h"

class Reflective : public Phong
{
public:
	Reflective(const Vec3& cd, float kd, const Vec3& cs, float ks, float exp);
	~Reflective();

	void set_rc(const Color& rc);

	virtual Vec3 shade_direct(ShadeInfo& r) override;

private:
	Color rc_;
};

