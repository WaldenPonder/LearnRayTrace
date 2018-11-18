#pragma once
#include "Vec3.h"

class Ray
{
public:
	Ray();
	Ray(const Vec3& o, const Vec3& d);
	~Ray();

	Point distance(float t) const;

public:
	Vec3 orig;
	Vec3 dir;
};

