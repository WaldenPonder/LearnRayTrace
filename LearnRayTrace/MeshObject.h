#pragma once
#include "Shape.h"

class MeshObject : public Shape
{
public:
	MeshObject(const string& filename);
	virtual ~MeshObject();

	virtual ShadeInfo intersect(const Ray& ray) override;

	Point offset;
	float magnify = 1.f;

private:
	void load();

	struct Impl;
	Impl* impl;
};

