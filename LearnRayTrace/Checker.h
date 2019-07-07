#pragma once
#include "Material.h"

class Checker : public Material
{
public:
	Checker();
	~Checker();

	virtual Vec3 shade(ShadeInfo& r) override;
	virtual Vec3 getColor(ShadeInfo& r) override;
};

