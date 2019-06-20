#include "stdafx.h"
#include "PerfectReflect.h"
#include <cmath>
#include "BRDF.h"
#include "MultiJittered.h"
#include "World.h"

PerfectReflect::PerfectReflect()
{
}


PerfectReflect::PerfectReflect(const Color& c) 
{
	_specularColor = _diffuseColor = c;
}

PerfectReflect::~PerfectReflect()
{
}

Color PerfectReflect::shade(ShadeInfo& info)
{
	Color val = getColor(info);

	Vec3 w = info.normal;
	Vec3 u = Vec3(0.00424, 1, 0.00764) ^ w;
	u.normalize();
	Vec3 v = u ^ w;

	Color f = g::Black;
	int count = 0;

	Vec3 wi = info.ray.dir - 2 * info.normal * info.ray.dir * info.normal;
	Ray r(info.position, wi);
	ShadeInfo rInfo(info.world->intersection(r));
	if (rInfo.valid())
	{
		f = rInfo.material->getColor(rInfo);
		count++;
	}
		
	Color c = f + val;// componentMultiply(f, val);

	return c.clamp(0, 1);
}

Color PerfectReflect::getColor(ShadeInfo& info)
{
	return _diffuseColor / PI;

	Vec3 lightDir(-info.position + Vec3(0, 2, -4.5));
	lightDir.normalize();

	float NdotL = info.normal * lightDir;
	Vec3 H = lightDir - info.ray.dir;
	H.normalize();

	float NdotH = info.normal * H;

	const float FACTOR = .7;
	Vec3 diffItem = _diffuseColor * max(NdotL, 0.f) * FACTOR;
	Vec3 speculaItem = _specularColor * pow(max(NdotL, .0f), _shiness) * (1 - FACTOR);
	Vec3 val = (diffItem + speculaItem);
	return val.clamp(0, 1);
}