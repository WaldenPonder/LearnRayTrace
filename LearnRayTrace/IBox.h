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
	Vec3 normal(1, 0, 0);
	int flag = 1;

	float tmin = (_minPt.x() - r.orig.x()) / r.dir.x();
#define TMIN tmin
	float tmax = (_maxPt.x() - r.orig.x()) / r.dir.x();

	if (TMIN > tmax)
	  std::swap(TMIN, tmax), normal = -normal;
		
	//-------------------------------------------------y
	flag = 1;
	float tymin = (_minPt.y() - r.orig.y()) / r.dir.y();
	float tymax = (_maxPt.y() - r.orig.y()) / r.dir.y();
	if (tymin > tymax)
		std::swap(tymin, tymax), flag = -1;
		
	if ((TMIN > tymax) || (tymin > tmax))
		return info;

	if (tymin > TMIN)
		TMIN = tymin, normal =  Vec3(0, flag, 0);
		
	if (tymax < tmax)
		tmax = tymax;

	//-------------------------------------------------z
	flag = 1;
	float tzmin = (_minPt.z() - r.orig.z()) / r.dir.z();
	float tzmax = (_maxPt.z() - r.orig.z()) / r.dir.z();

	if (tzmin > tzmax)
		std::swap(tzmin, tzmax), flag = -1;
		
	if ((TMIN > tzmax) || (tzmin > tmax))
		return info;

	if (tzmin > TMIN)
		TMIN = tzmin, normal = Vec3(0, 0, flag);
		
	if (tzmax < tmax)
		tmax = tzmax;

	info.dis = TMIN;
	info.hit_pos = r.distance(TMIN);
	info.ray = r;
	info.normal = -normal;
	info.setShape(this);
	return info;
}

