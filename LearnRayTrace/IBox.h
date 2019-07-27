#pragma once
#include "Shape.h"

class Ray;

class IBox : public Shape
{
public:
	__class__(IBox);

	virtual Vec3 center() const { return Vec3(); }
	virtual ShadeInfo intersect(const Ray& ray);

	Vec3 _minPt;
	Vec3 _maxPt;
};

inline ShadeInfo IBox::intersect(const Ray& r)
{
	ShadeInfo info;
	Vec3 normal = Vec3(1, 0, 0);
	float tmin = (_minPt.x() - r.orig.x()) / r.dir.x();
	float tmax = (_maxPt.x() - r.orig.x()) / r.dir.x();

	if (tmin > tmax)
	{
		std::swap(tmin, tmax);
		normal = -normal;
	}
		
	float tymin = (_minPt.y() - r.orig.y()) / r.dir.y();
	float tymax = (_maxPt.y() - r.orig.y()) / r.dir.y();
	Vec3 normaly = Vec3(0, 1, 0);
	if (tymin > tymax)
	{
		normaly = -normaly;
		std::swap(tymin, tymax);
	}
		
	if ((tmin > tymax) || (tymin > tmax))
		return info;

	if (tymin > tmin)
	{
		normal = normaly;
		tmin = tymin;
	}
		
	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (_minPt.z() - r.orig.z()) / r.dir.z();
	float tzmax = (_maxPt.z() - r.orig.z()) / r.dir.z();
	Vec3 normalz = Vec3(0, 0, 1);

	if (tzmin > tzmax)
	{
		normalz = -normalz;
		std::swap(tzmin, tzmax);
	}
		
	if ((tmin > tzmax) || (tzmin > tmax))
		return info;

	if (tzmin > tmin)
	{
		tmin = tzmin;
		normal = normalz;
	}
		
	if (tzmax < tmax)
		tmax = tzmax;

	info.dis = tmin;
	info.hit_pos = r.distance(tmin);
	info.ray = r;
	info.normal = normal;

	info.setShape(this);
	return info;
}

