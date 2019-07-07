#pragma once
#include "Material.h"

class Emissive : public Material
{
public:
	Emissive();
	~Emissive();

	virtual Vec3 shade(ShadeInfo& r) override;
	virtual Vec3 getColor(ShadeInfo& r) override;

	Vec3 _light = g::White;
};

