#pragma once
#include "Shape.h"

class MeshObject : public Shape
{
public:
	MeshObject(const string& filename);
	virtual ~MeshObject();

	virtual ShadeInfo intersect(const Ray& ray) override;

private:
	void load();
	struct Impl;
	Impl* impl;
};

