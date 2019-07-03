#pragma once
#include "BBox.h"
#include "MeshObject.h"

//14ÃæÌå
class Extent : public BBox
{
 public:
	meta_name(Extent);
	Extent();
	Extent(MeshObject* mesh);
	Extent(const Extent& pbb);

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

	MeshObject* mesh_ = nullptr;
	float		slabs_[7][2];
};
