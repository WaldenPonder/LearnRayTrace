#pragma once
#include "Material.h"

class BRDF;

class Matte : public Material
{
public:
	Matte();
	Matte(const Color& c);
	~Matte();

	virtual Color shade(ShadeInfo& r) override;
	virtual Color getColor(ShadeInfo& r) override;

	BRDF* _brdf = nullptr;
};

