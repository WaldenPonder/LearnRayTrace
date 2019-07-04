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
		material = s->_material;
	}
}

bool ShadeInfo::valid()
{
	return shape != nullptr;
}