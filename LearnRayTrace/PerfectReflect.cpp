#include "stdafx.h"
#include "PerfectReflect.h"
#include <cmath>
#include "BRDF.h"
#include "MultiJittered.h"
#include "World.h"

//-------------------------------------------------
Vec3 _diffuseColor, _specularColor;

PerfectReflect::PerfectReflect()
{
}

PerfectReflect::~PerfectReflect()
{
}

Vec3 PerfectReflect::shade(ShadeInfo& info)
{
	Vec3 val = _diffuseColor;

	Vec3 f = g::Black;

	Vec3 wi = info.ray.dir - 2 * info.normal * info.ray.dir * info.normal;
	Ray r(info.position, wi);
		
	ShadeInfo rInfo = World::Instance()->intersection(r);
	f = World::Instance()->trace_ray(r, info.depth + 1);
	
	Vec3 c = componentMultiply(f, val);
	return c;
}

Vec3 PerfectReflect::shade_direct(ShadeInfo& info)
{
	Vec3 lightDir(-info.position + Vec3(0, 2, -4.5));
	lightDir.normalize();

	float NdotL = info.normal * lightDir;
	Vec3 H = lightDir - info.ray.dir;
	H.normalize();

	float NdotH = info.normal * H;

	const float FACTOR = .7;
	Vec3 diffItem = _diffuseColor * max(NdotL, 0.f) * FACTOR;
	Vec3 speculaItem = _specularColor * pow(max(NdotL, .0f), 30) * (1 - FACTOR);
	Vec3 val = (diffItem + speculaItem);
	return val.clamp(0, 1);
}