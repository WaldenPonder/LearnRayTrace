#pragma once
#include "Vec3.h"
#include "ShadeInfo.h"
#include "common.h"

class Material
{
public:
	Material();
	~Material();

	virtual Color shade(ShadeInfo& r) = 0;
	virtual Color getColor(ShadeInfo& r) { return g::Black; }

	Vec3  _specularColor;
	Vec3  _diffuseColor;
	float _shiness = 32;
	float _indirect_factor = .3f; //间接光贡献比
};

