#include "stdafx.h"
#include "Phong.h"
#include <cmath>

Phong::Phong()
{
}


Phong::Phong(const Color& c) 
{
	_specularColor = _diffuseColor = c;
}

Phong::~Phong()
{
}

Color Phong::shade(ShadeInfo& r)
{
	Vec3 lightDir(-r.position + Vec3(0, .7, 0));
	lightDir.normalize();

	float NdotL = r.normal * lightDir;
	Vec3 H = lightDir - r.ray.dir;
	H.normalize();

	float NdotH = r.normal * H;

	Vec3 diffItem = _diffuseColor * max(NdotL, 0.f);
	Vec3 speculaItem = _specularColor * pow(max(NdotL, .0f), _shiness);
	Vec3 val = (diffItem + speculaItem + g::White * .1);

	return val.clamp(0.f, 255.f) / 255.0;
}
