#pragma once
#include "Material.h"

class BRDF;

class Phong : public Material
{
public:
	Phong();
	Phong(const Color& c);
	~Phong();

	virtual Color shade(ShadeInfo& r) override;

	BRDF* _brdf = nullptr;
};

