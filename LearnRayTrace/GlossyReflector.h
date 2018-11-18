#pragma once
#include "Phong.h"

class GlossyReflector : public Phong
{
public:
	GlossyReflector();
	GlossyReflector(const Color& c);
	~GlossyReflector();

	virtual Color shade(ShadeInfo& info) override;
};

