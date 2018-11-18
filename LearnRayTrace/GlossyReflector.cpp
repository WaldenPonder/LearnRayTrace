#include "stdafx.h"
#include "World.h"
#include "GlossyReflector.h"
#include "GlossySpecular.h"

GlossyReflector::GlossyReflector()
{
	_brdf = new GlossySpecular;
	_brdf->_material = this;
}


GlossyReflector::GlossyReflector(const Color& c) : Phong(c)
{
	_brdf = new GlossySpecular;
	_brdf->_material = this;
}

GlossyReflector::~GlossyReflector()
{
}

Color GlossyReflector::shade(ShadeInfo& info)
{
	
#if 0

	Color L = Phong::shade(info);
	return L;

	Vec3 wo = -info.ray.dir;

	Vec3 wi;
	float pdf;
	Color fr = _brdf->sample_f(info, wo, wi, pdf);
	
	Ray reflected_ray(info.position, wi);

	Color c1 = componentMultiply(fr, info.world->trace_ray(reflected_ray, info.depth + 1));
	
	return componentMultiply(c1, L);

#else

	Vec3 wo = -info.ray.dir;
	Vec3 wi;
	float pdf;
	Color f = _brdf->sample_f(info, wo, wi, pdf);

	float ndowi = info.normal * wi;
	Ray reflected_ray(info.position, wi);

	Color c1 = info.world->trace_ray(reflected_ray, info.depth + 1);
	
#ifdef USING_COUT

	cout << endl << endl << " { " << endl;
	cout << "SAMPLE_F  " << f << endl;
	cout << "DEPTH  " << (info.depth + 1) << c1 << endl;
	cout << " COMPONENTMULTIPLY   " << componentMultiply(f, c1) << endl;

#endif
	//Color L = Phong::shade(info);

	Color rtn = componentMultiply(f, c1) * ndowi / pdf;
	
	//rtn = componentMultiply(L, rtn);

#ifdef USING_COUT

	cout << "NDOWI  " << ndowi << "   PDF " << pdf << endl;
	cout << "COLOR " << rtn << endl;
	cout << "} " << endl;

#endif

	return rtn;

#endif
}
