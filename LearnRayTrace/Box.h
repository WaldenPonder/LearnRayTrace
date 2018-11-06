#pragma once
#include "Shape.h"

class Box : public Shape
{
public:
	Box(const Vec3& loPt, int size);
	~Box();

	virtual ShadeInfo intersect(const Ray& ray) override;

	Vec3 _minPt;
	Vec3 _maxPt;
};

