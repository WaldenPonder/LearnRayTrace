#pragma once
#include "Matte.h"
#include "Vec3.h"   // for Color

class GlossyReflector : public Matte
{
public:
	GlossyReflector(const Vec3& c, int k);
	~GlossyReflector();

	virtual Vec3 shade(ShadeInfo& info) override;
};

