#include "stdafx.h"
#include "Box.h"

Box::Box(const Vec3& loPt, int size)
{
	_minPt = loPt;
	_maxPt = loPt + Vec3(size);
}

Box::~Box()
{
}

ShadeInfo Box::intersect(const Ray& r)
{
	ShadeInfo info;

	float tmin = (_minPt.x() - r.orig.x()) / r.dir.x();
	float tmax = (_maxPt.x() - r.orig.x()) / r.dir.x();

	if (tmin > tmax) std::swap(tmin, tmax);

	float tymin = (_minPt.y() - r.orig.y()) / r.dir.y();
	float tymax = (_maxPt.y() - r.orig.y()) / r.dir.y();

	if (tymin > tymax) std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return info;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (_minPt.z() - r.orig.z()) / r.dir.z();
	float tzmax = (_maxPt.z() - r.orig.z()) / r.dir.z();

	if (tzmin > tzmax) std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return info;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	info.setShape(this);
	info.distance = tmin;

	return info;
}
