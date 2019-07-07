#pragma once
#include "BRDF.h"

class Lambert : public BRDF
{
public:
	Lambert();
	virtual ~Lambert();

	virtual Vec3 f() const;

	Color diffuse_;
};

