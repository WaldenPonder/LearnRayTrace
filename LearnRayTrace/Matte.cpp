#include "stdafx.h"
#include "Matte.h"
#include <cmath>
#include "BRDF.h"
#include "MultiJittered.h"
#include "World.h"
#include "Light.h"
#include "PhotonMap.h"

Matte::Matte(const Vec3& c, float k) : brdf_(c, k)
{
}

Matte::~Matte()
{
}

Vec3 Matte::shade(ShadeInfo& si)
{
	bool bRet;
	Vec3 f = lambert_f(si, bRet);
	if (bRet) return f;

	Vec3 w = si.normal;
	Vec3 u = Vec3(0.00424, 1, 0.00764) ^ w;
	u.normalize();
	Vec3 v = u ^ w;

	Vec3 c = g::Black;

	//------------------------------------------------------cosine distribution
#if 1
	//Vec3 sp = MultiJittered::instance()->sample_hemisphere();
	Vec3 sp = g::random_cosine_direction();
	Vec3 wi = sp.x() * u + sp.y() * v + sp.z() * w;  // reflected ray direction
	Ray  r(si.hit_pos, wi);

	f = si.normal * wi * f;

	c = World::Instance()->trace_ray(r, si.depth + 1);
#else
	Vec3  lightPos(rand_float(-0.5, 0.5), 1.84, rand_float(-5, -4));
	Vec3  lightDir	= lightPos - si.hit_pos;
	float distance	= lightDir.normalize();
	float lightCosine = fabs(lightDir.y());

	if (si.normal * lightDir < 0 || lightCosine < kEpsilon) return Vec3();

	float lightArea = 1;
	float pdf		= distance * distance / (lightArea * lightCosine);
	f				= Vec3(1.f / pdf);

	Vec3 sp = lightDir;
	Vec3 wi = sp.x() * u + sp.y() * v + sp.z() * w;  // reflected ray direction
	Ray  r(si.hit_pos, wi);

	c = World::Instance()->trace_ray(r, si.depth + 1);

#endif

	Vec3 res = componentMultiply(f, c);
	return res;
}

Vec3 Matte::shade_direct(ShadeInfo& info)
{
	Vec3 val = World::Instance()->get_ambient();

	for (Light* light : Light::pool())
	{
		Vec3  lightDir = light->getDir(info);
		float NdotL	= info.normal * lightDir;

		Vec3 diffItem = brdf_.f() * max(NdotL, 0.f);
		val += diffItem;
	}

	if (World::Instance()->isInShadow(info))
		val *= .1;

	return val.clamp(0, 1);
}

void Matte::collect_photon(ShadeInfo& si, Vec3 color)
{
	bool bRet;
	Vec3 f = lambert_f(si, bRet);
	if (bRet) return;

	//if (rand_float() > (f.x() + f.y() + f.z()) / 3.)
	//	return;

	Vec3 w = si.normal;
	Vec3 u = Vec3(0.00424, 1, 0.00764) ^ w;
	u.normalize();
	Vec3 v = u ^ w;

	//------------------------------------------------------cosine distribution
	Vec3 sp = g::random_cosine_direction();
	Vec3 wi = sp.x() * u + sp.y() * v + sp.z() * w;  // reflected ray direction
	Ray  r(si.hit_pos, wi);

	f = si.normal * wi * f;

	Vec3 c = componentMultiply(f, color);
	PhotonMap::Instance()->addPhoton(Photon{ si.hit_pos, wi, c });

	World::Instance()->collect_photon(r, si.depth + 1, c);
}

Vec3 Matte::lambert_f(ShadeInfo& si, bool& bRet) const
{
	bRet	 = false;
	Vec3 val = brdf_.f();

	if (si.depth > 5)
	{
		float f = max({ val.x(), val.y(), val.z() });
		if (rand_float() < f)
			val = val / (1 / f);
		else
		{
			bRet = true;
			return Vec3();
		}
	}

	if (si.depth > 30)
	{
		bRet = true;
		return Vec3();
	}

	return val;
}
