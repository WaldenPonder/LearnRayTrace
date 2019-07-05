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

	virtual Vec3 center() const override;

	virtual bool intersect(const Ray& ray) override;

	float max_len() const;
	float min_len() const;

	Vec3 _minPt = Vec3(FLT_MAX);
	Vec3 _maxPt = Vec3(-FLT_MAX);
};