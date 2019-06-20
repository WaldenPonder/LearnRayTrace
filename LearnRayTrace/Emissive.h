#pragma once
#include "Material.h"

class Emissive : public Material
{
public:
	Emissive();
	~Emissive();

	virtual Color shade(ShadeInfo& r) override;
	virtual Color getColor(ShadeInfo& r) override;

	Color _light = g::White;
};

