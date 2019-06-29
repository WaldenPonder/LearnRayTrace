#pragma once
#include "Material.h"

class Checker : public Material
{
public:
	Checker();
	~Checker();

	virtual Color shade(ShadeInfo& r) override;
	virtual Color getColor(ShadeInfo& r) override;
};

