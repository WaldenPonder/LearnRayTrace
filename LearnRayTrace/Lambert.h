#pragma once
#include "BRDF.h"

class Lambert : public BRDF
{
public:
	Lambert(const Vec3& c, float k);
	virtual ~Lambert();

	virtual Vec3 f() const;

private:
	Color diffuse_;
};

