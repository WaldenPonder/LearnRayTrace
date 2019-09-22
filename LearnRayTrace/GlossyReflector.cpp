#include "stdafx.h"
#include "GlossyReflector.h"
#include "BRDF.h"  // for BRDF
#include "GlossySpecular.h"
#include "World.h"

GlossyReflector::GlossyReflector(const Vec3& c, int k) : Matte(c, k)
{

}

GlossyReflector::~GlossyReflector() {}

Vec3 GlossyReflector::shade(ShadeInfo& si)
{
	Vec3 L;

	const int kSIZE = 1;

	for (int i = 0; i < kSIZE; i++)
	{
		Vec3  wo = -si.ray.dir;
		Vec3  wi;
		float pdf;
		Vec3  f = _brdf->sample_f(si, wo, wi, pdf);

		float ndowi = si.normal * wi;
		Ray   reflected_ray(si.hit_pos, wi, si.ray.depth + 1);

		Vec3 c1 = World::Instance()->trace_ray(reflected_ray);
		L += (componentMultiply(f, c1)) * ndowi / pdf;
	}

	return L / kSIZE;
}
