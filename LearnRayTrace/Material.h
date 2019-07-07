#pragma once
#include "Vec3.h"
#include "ShadeInfo.h"
#include "Lambert.h"

class Material
{
public:
	Material();
	~Material();

	virtual Vec3 shade(ShadeInfo& r) = 0;
	virtual Vec3 getColor(ShadeInfo& r) { return g::Black; }

	BRDF* _brdf = nullptr;
};
