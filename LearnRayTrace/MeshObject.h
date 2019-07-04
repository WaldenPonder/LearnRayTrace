#pragma once
#include "Shape.h"
#include "Matrix.h"
#include "BoundingBox.h"

class Mesh
{
 public:
	meta_name(Mesh);
	Mesh(const string& filename);
	~Mesh();

 private:
	void load();

	struct Impl;
	Impl* impl;
	friend class MeshObject;
};

class MeshObject : public Shape
{
 public:
	meta_name(MeshObject);
	MeshObject(const Mesh& mesh);
	virtual ~MeshObject();

	virtual ShadeInfo intersect(const Ray& ray) override;

	void init_polytope_boundingbox(Extent& extent) const;

	Matrix matrix_;

 private:
	void computBBox();

	BoundingBox bbox_;
	const Mesh& mesh_;
};
