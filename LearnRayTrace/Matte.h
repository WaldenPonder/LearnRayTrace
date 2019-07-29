#pragma once
#include "Material.h"


class Matte : public Material
{
public:
	Matte(const Vec3& c, float k);
	~Matte();

	virtual Vec3 shade(ShadeInfo& r) override;
	virtual Vec3 shade_direct(ShadeInfo& r) override;
	Vec3 lambert_f(ShadeInfo& si, bool& bRet) const;

private:
	Lambert brdf_;
};

