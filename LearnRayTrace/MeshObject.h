#pragma once
#include "Shape.h"


class MeshObject : public Shape
{
public:
	meta_name(MeshObject);
	MeshObject(const string& filename);
	virtual ~MeshObject();

	virtual ShadeInfo intersect(const Ray& ray) override;

	void init_polytope_boundingbox(Extent& extent) const;

	Point offset;
	float magnify = 1.f;

private:
	void load();
	void computBBox();

	struct Impl;
	Impl* impl;
};

