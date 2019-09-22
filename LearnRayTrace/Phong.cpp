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

Vec3 Phong::shade(ShadeInfo& si)
{
	Vec3 L(Matte::shade(si));

	return L;
}

Vec3 Phong::shade_direct(ShadeInfo& si)
{
	Vec3 L(Matte::shade_direct(si));

	float k = 1.f;// World::Instance()->isInShadow(si) ? .1 : 1;

	for (Light* light : Light::pool())
	{
		Vec3 lightDir = light->getDir(si);
		float NdotL = si.normal * lightDir;

		L += glossySpecular_.f(si, -si.ray.dir, lightDir) * k * max(NdotL, 0.f);
	}
	return L.clamp(0, 1);
}
