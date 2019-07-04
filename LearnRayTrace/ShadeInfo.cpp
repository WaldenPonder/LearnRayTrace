#include "stdafx.h"
#include "Shape.h"

ShadeInfo::ShadeInfo()
{
	dis = FLT_MAX;
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