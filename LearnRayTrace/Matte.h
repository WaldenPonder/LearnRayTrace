#pragma once
#include "Material.h"


class Matte : public Material
{
public:
	Matte();
	~Matte();

	virtual Vec3 shade(ShadeInfo& r) override;
	virtual Vec3 getColor(ShadeInfo& r) override;

	Lambert diffuse_brdf;
};

