#include "stdafx.h"
#include "World.h"


ShadeInfo::ShadeInfo()
{
	world = World::Instance();
	distance = DBL_MAX;
}

void ShadeInfo::setShape(Shape* s)
{
	if (s)
	{
		shape = s;
		material = s->_material;
	}
}

ShadeInfo::operator bool() const
{
	return shape != nullptr;
}
