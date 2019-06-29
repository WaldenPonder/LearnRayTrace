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
	Color val = _diffuseColor;

	Color f = g::Black;

	Vec3 wi = info.ray.dir - 2 * info.normal * info.ray.dir * info.normal;
	Ray r(info.position, wi);
		
	ShadeInfo rInfo = info.world->intersection(r);
	f = info.world->trace_ray(r, info.depth + 1);
	
	Color c = componentMultiply(f, val);
	return c;
}

Color PerfectReflect::getColor(ShadeInfo& info)
{
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