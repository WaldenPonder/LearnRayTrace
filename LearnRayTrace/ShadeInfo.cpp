#include "stdafx.h"
#include "Shape.h"

ShadeInfo::ShadeInfo()
{
}

Vec3 ShadeInfo::reflect() const
{
	Vec3 wo = -ray.dir;
	Vec3 wi = -wo + 2. * normal * wo * normal;
	wi.normalize();
	return wi;
}

void ShadeInfo::setShape(Shape* s)
{
	if (s)
	{
		shape	= s;
		material = s->material_;
	}
}

bool ShadeInfo::valid()
{
	return shape != nullptr;
}