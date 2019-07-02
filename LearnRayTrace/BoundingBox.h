#pragma once
#include "BBox.h"

class BoundingBox : public BBox
{
public:
	BoundingBox();
	~BoundingBox();

	Vec3& operator [] (bool i) { return _minPt; }
	const Vec3 operator [] (bool i) const { return _maxPt; }

	void reset();

	bool valid() const;

	void expandBy(const Vec3& pt);

	virtual bool intersect(const Ray& ray) override;

	Vec3 _minPt = Vec3(FLT_MAX);
	Vec3 _maxPt = Vec3(-FLT_MAX);
};