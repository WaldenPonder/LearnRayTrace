#include "stdafx.h"
#include "Phong.h"
#include <cmath>
#include "BRDF.h"
#include "MultiJittered.h"
#include "World.h"

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

#define SIZE 200

Color Phong::shade(ShadeInfo& info)
{
	Color val = getColor(info);

	Vec3 w = info.normal;
	Vec3 u = Vec3(0.00424, 1, 0.00764) ^ w;
	u.normalize();
	Vec3 v = u ^ w;

	Color f = g::Black;
	int count = 0;
	for (int i = 0; i < SIZE; i++)
	{
		Point sp = MultiJittered::instance()->sample_hemisphere();
		Vec3 wi = sp.x() * u + sp.y() * v + sp.z() * w;			// reflected ray direction
		Ray r(info.position, wi);

		f += info.world->trace_ray(r, info.depth + 1);
		count++;
	}

	f /= count;
	Color c = val + f; // componentMultiply(val, f);

	return c.clamp(0, 1);
}

Color Phong::getColor(ShadeInfo& info)
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