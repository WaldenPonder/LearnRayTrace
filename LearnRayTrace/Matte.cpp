#include "stdafx.h"
#include "Matte.h"
#include <cmath>
#include "BRDF.h"
#include "MultiJittered.h"
#include "World.h"

Matte::Matte()
{
}


Matte::Matte(const Color& c) 
{
	_specularColor = _diffuseColor = c;
}

Matte::~Matte()
{
}

Color Matte::shade(ShadeInfo& info)
{
	Color val = _diffuseColor;

	if (info.depth > 5)
	{
		float f = max(_diffuseColor.x(), _diffuseColor.y());
		f = max(f, _diffuseColor.z());

		if (rand_float() < f) val /= f;
		else return Color();
	}

	if (info.depth > 100)
		return Color();
	
	Vec3 w = info.normal;
	Vec3 u = Vec3(0.00424, 1, 0.00764) ^ w;
	u.normalize();
	Vec3 v = u ^ w;

	Color f = g::Black;

	Point sp = MultiJittered::instance()->sample_hemisphere();
	Vec3 wi = sp.x() * u + sp.y() * v + sp.z() * w;			// reflected ray direction
	Ray r(info.position, wi);

	f = World::Instance()->trace_ray(r, info.depth + 1);

	Color c = componentMultiply(val, f);
	return c;
}

Color Matte::getColor(ShadeInfo& info)
{
	Vec3 lightDir(-info.position + Vec3(0, 1, -4.5));
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