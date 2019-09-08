#pragma once
#include "Material.h"


class Matte : public Material
{
public:
	Matte(const Vec3& c, float k);
	~Matte();

	virtual Vec3 shade(ShadeInfo& si) override;
	virtual Vec3 shade_direct(ShadeInfo& si) override;
	virtual void collect_photon(ShadeInfo& si, Vec3 color) override;

	Vec3 lambert_f(ShadeInfo& si, bool& bRet) const;

private:
	Lambert brdf_;
};

