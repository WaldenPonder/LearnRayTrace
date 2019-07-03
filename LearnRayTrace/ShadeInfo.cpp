#include "stdafx.h"
#include "World.h"

ShadeInfo::ShadeInfo() {
  world = World::Instance();
  distance = FLT_MAX;
}

void ShadeInfo::setShape(Shape *s) {
  if (s) {
    shape = s;
    material = s->_material;
  }
}

bool ShadeInfo::valid()
{
	return shape != nullptr;
}