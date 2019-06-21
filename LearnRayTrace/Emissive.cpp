#include "stdafx.h"
#include "Emissive.h"


Emissive::Emissive()
{
}


Emissive::~Emissive()
{
}

Color Emissive::shade(ShadeInfo& r)
{
	return _light * 10;
}

Color Emissive::getColor(ShadeInfo& r)
{
	return _light * 10;
}