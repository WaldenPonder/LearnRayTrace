#pragma once
#include "Object.h"

//14����
class Extent : public Object
{
 public:
	__class__(Extent);
	Extent();
	explicit Extent(MeshObject* mesh);
	Extent(const Extent& extent);

	void operator=(const Extent& extent);

	virtual ~Extent();

	virtual bool is_intersect(const Ray& ray);
	bool		 intersect(
				const float* precomputedNumerator,
				const float* precomputedDenominator,
				float&		 tNear,  // tn and tf in this method need to be contained
				float&		 tFar,   // within the range [tNear:tFar]
				uint8_t&	 planeIndex) const;

	virtual Vec3 center() const;
	void		 extendBy(const Extent& pbb);

	void init();
	bool valid() const;

	MeshObject* mesh_ = nullptr;
	float		slabs_[7][2];
};
