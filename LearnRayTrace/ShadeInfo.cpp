#include "stdafx.h"
#include "World.h"


ShadeInfo::ShadeInfo()
{
	world = World::Instance();
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
