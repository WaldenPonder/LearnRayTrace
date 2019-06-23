#include "GlossyReflector.h"
#include "BRDF.h" // for BRDF
#include "GlossySpecular.h"
#include "World.h"
#include "stdafx.h"

GlossyReflector::GlossyReflector() {
  _brdf = new GlossySpecular;
  _brdf->_material = this;
}

GlossyReflector::GlossyReflector(const Color &c) : Matte(c) {
  _brdf = new GlossySpecular;
  _brdf->_material = this;
}

GlossyReflector::~GlossyReflector() {}

Color GlossyReflector::shade(ShadeInfo &info) {

	Color rtn;

	const int kSIZE = 1;

	for (int i = 0; i < kSIZE; i++)
	{
		Vec3 wo = -info.ray.dir;
		Vec3 wi;
		float pdf;
		Color f = _brdf->sample_f(info, wo, wi, pdf);

		float ndowi = info.normal * wi;
		Ray reflected_ray(info.position, wi);

		Color c1 = info.world->trace_ray(reflected_ray, info.depth + 1);
		rtn += (componentMultiply(f, c1)) *ndowi / pdf;
	}

  return rtn / kSIZE;
}
