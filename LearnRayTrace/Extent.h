#pragma once
#include "IBox.h"

//14ÃæÌå
class Extent : public IBox
{
 public:
	__class__(Extent);
	Extent();
	explicit Extent(MeshObject* mesh);
	Extent(const Extent& extent);

	void operator=(const Extent& extent);

	virtual ~Extent();

	virtual bool intersect(const Ray& ray) override;
	bool		 intersect(
				const float* precomputedNumerator,
				const float* precomputedDenominator,
				float&		 tNear,  // tn and tf in this method need to be contained
				float&		 tFar,   // within the range [tNear:tFar]
				uint8_t&	 planeIndex) const;

	virtual Vec3 center() const override;
	void		 extendBy(const Extent& pbb);

	void init();
	bool valid() const;

	MeshObject* mesh_ = nullptr;
	float		slabs_[7][2];
};
