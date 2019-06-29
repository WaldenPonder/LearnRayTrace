#include "stdafx.h"
#include "Checker.h"


Checker::Checker()
{
}


Checker::~Checker()
{
}

Color Checker::shade(ShadeInfo& r)
{
	return getColor(r);
}

Color Checker::getColor(ShadeInfo& r)
{
	static float factor = .05;
	return fabs((int)(floor(r.position.x() * factor) + floor(r.position.z() * factor)) % 2) < 1 ? g::Black : g::White;
}
