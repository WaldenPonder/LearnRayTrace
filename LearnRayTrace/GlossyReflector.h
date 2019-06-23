#pragma once
#include "Matte.h"
#include "Vec3.h"   // for Color

class GlossyReflector : public Matte
{
public:
	GlossyReflector();
	GlossyReflector(const Color& c);
	~GlossyReflector();

	virtual Color shade(ShadeInfo& info) override;
};

