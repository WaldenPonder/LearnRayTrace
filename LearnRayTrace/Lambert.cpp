#include "stdafx.h"
#include "Lambert.h"

Lambert::Lambert(const Vec3& c, float k)
{
	diffuse_.c = c, diffuse_.k = k;
}

Lambert::~Lambert()
{
}

Vec3 Lambert::f() const
{
	return diffuse_.get() / PI;
}
