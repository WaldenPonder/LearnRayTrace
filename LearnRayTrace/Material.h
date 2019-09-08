#pragma once
#include "Vec3.h"
#include "ShadeInfo.h"
#include "Lambert.h"

class Material : public Object
{
public:
	__class__(Material);
	Material();
	~Material();

	virtual Vec3 shade(ShadeInfo& si) = 0;
	virtual Vec3 shade_direct(ShadeInfo& si) { return g::Black; }
	virtual void collect_photon(ShadeInfo& si, Vec3 color) {  }

	BRDF* _brdf = nullptr;
};
