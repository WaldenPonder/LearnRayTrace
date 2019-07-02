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
	Vec3 center() const;
	void extendBy(const Extent& pbb);

	MeshObject* mesh_ = nullptr;
	float slabs_[7][2];
};

