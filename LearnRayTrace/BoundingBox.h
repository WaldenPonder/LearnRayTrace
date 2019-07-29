#pragma once
#include "IBox.h"

class BoundingBox : public Object
{
public:
	__class__(BoundingBox);
	BoundingBox();
	~BoundingBox();

	Vec3& operator [] (bool i) { return _minPt; }
	const Vec3 operator [] (bool i) const { return _maxPt; }

	void reset();

	bool valid() const;

	void expandBy(const Vec3& pt);

	virtual Vec3 center() const;

	virtual bool is_intersect(const Ray& ray);

	float max_len() const;
	float min_len() const;

	Vec3 _minPt;
	Vec3 _maxPt;
};