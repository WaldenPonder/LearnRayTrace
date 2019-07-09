#include "stdafx.h"
#include "Matte.h"
#include <cmath>
#include "BRDF.h"
#include "MultiJittered.h"
#include "World.h"
#include "Light.h"

Matte::Matte()
{
}

Matte::Matte(const Vec3& c, float k)
{
	diffuse_brdf.diffuse_.c = c;
	diffuse_brdf.diffuse_.k = k;
}

Matte::~Matte()
{
}

Vec3 Matte::shade(ShadeInfo& info)
{
	Vec3 val = diffuse_brdf.f();

	if (info.depth > 5)
	{
		float f = max({ val.x(), val.y(), val.z()} );
		if (rand_float() < f) val /= f;
		else return Vec3();
	}

	if (info.depth > 100)
		return Vec3();
	
	Vec3 w = info.normal;
	Vec3 u = Vec3(0.00424, 1, 0.00764) ^ w;
	u.normalize();
	Vec3 v = u ^ w;

	Vec3 f = g::Black;

	Point sp = MultiJittered::instance()->sample_hemisphere();
	Vec3 wi = sp.x() * u + sp.y() * v + sp.z() * w;			// reflected ray direction
	Ray r(info.position, wi);

	f = World::Instance()->trace_ray(r, info.depth + 1);

	Vec3 c = componentMultiply(val, f);
	return c;
}

Vec3 Matte::shade_direct(ShadeInfo& info)
{	
	Vec3 val = World::Instance()->get_ambient();

	for (Light* light : Light::pool())
	{
		Vec3 lightDir = light->getDir(info);
		float NdotL = info.normal * lightDir;

		Vec3 diffItem = diffuse_brdf.f() * max(NdotL, 0.f);
		val += diffItem;
	}
	
	return val.clamp(0, 1);
}