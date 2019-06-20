#pragma once
#include "Material.h"

class BRDF;

class PerfectReflect : public Material
{
public:
	PerfectReflect();
	PerfectReflect(const Color& c);
	~PerfectReflect();

	virtual Color shade(ShadeInfo& r) override;
	virtual Color getColor(ShadeInfo& r) override;

	BRDF* _brdf = nullptr;
};

