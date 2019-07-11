#include "stdafx.h"
#include "Phong.h"
#include "Light.h"
#include "World.h"

Phong::Phong(const Vec3& cd, float kd, const Vec3& cs, float ks, float exp)
	: Matte(cd, kd), glossySpecular_(Color(cs, ks), exp)
{

}

Phong::~Phong()
{
}

Vec3 Phong::shade(ShadeInfo& r)
{
	Vec3 L(Matte::shade(r));

	return L;
}

Vec3 Phong::shade_direct(ShadeInfo& info)
{
	Vec3 L(Matte::shade_direct(info));

	float k = World::Instance()->isInShadow(info) ? .1 : 1;

	for (Light* light : Light::pool())
	{
		Vec3 lightDir = light->getDir(info);
		float NdotL = info.normal * lightDir;

		L += glossySpecular_.f(info, -info.ray.dir, lightDir) * k * max(NdotL, 0.f);
	}
	return L.clamp(0, 1);
}
