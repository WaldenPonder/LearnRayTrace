#pragma once
#include "Material.h"

class BRDF;

class PerfectReflect : public Material
{
public:
	PerfectReflect();
	~PerfectReflect();

	virtual Vec3 shade(ShadeInfo& r) override;
	virtual Vec3 shade_direct(ShadeInfo& r) override;
};

