#pragma once
#include "Phong.h"
#include "Vec3.h"   // for Color

class GlossyReflector : public Phong
{
public:
	GlossyReflector();
	GlossyReflector(const Color& c);
	~GlossyReflector();

	virtual Color shade(ShadeInfo& info) override;
};

