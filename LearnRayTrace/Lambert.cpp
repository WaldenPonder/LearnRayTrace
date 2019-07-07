#include "stdafx.h"
#include "Lambert.h"


Lambert::Lambert()
{
}


Lambert::~Lambert()
{
}

Vec3 Lambert::f() const
{
	return diffuse_.get() / PI;
}
