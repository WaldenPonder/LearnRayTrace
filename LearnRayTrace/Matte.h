#pragma once
#include "Material.h"


class Matte : public Material
{
public:
	Matte();
	~Matte();

	virtual Vec3 shade(ShadeInfo& r) override;
	virtual Vec3 shade_direct(ShadeInfo& r) override;

	Lambert diffuse_brdf;
};

