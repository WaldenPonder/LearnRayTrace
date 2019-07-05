#include "stdafx.h"
#include "MeshObject.h"
#include "tiny_obj_loader.h"
#include "util.h"
#include "BoundingBox.h"
#include "Extent.h"
#include "World.h"

struct Mesh::Impl
{
	tinyobj::attrib_t				 attrib;
	std::vector<tinyobj::shape_t>	shapes;
	std::vector<tinyobj::material_t> materials;
	bool							 triangulate = true;
	string							 filename;	
};

struct MeshObject::Impl
{
	Impl(const Mesh& mesh) : mesh_(mesh) {}
	BoundingBox bbox_;
	Extent extent_;
	const Mesh& mesh_;

	bool uesExtent = true;
};

Mesh::Mesh(const string& filename) : impl(new Impl)
{
	impl->filename = filename;
	load();
}

Mesh::~Mesh()
{
	delete impl;
}

void Mesh::load()
{
	std::string warn;
	std::string err;

	bool ret = tinyobj::LoadObj(&impl->attrib, &impl->shapes, &impl->materials, &warn, &err, impl->filename.c_str(),
								NULL, impl->triangulate);

	if (!ret)
	{
		cout << "load " << impl->filename << " fail\n";
	}
}

MeshObject::MeshObject(const Mesh& mesh) : impl(new Impl(mesh))
{
	impl->extent_.mesh_ = this;
}

MeshObject::~MeshObject()
{
}

void MeshObject::computBBox()
{
	const Mesh& mesh = impl->mesh_;
	for (tinyobj::shape_t& shape : mesh.impl->shapes)
	{
		std::vector<tinyobj::index_t>& index	= shape.mesh.indices;
		std::vector<tinyobj::real_t>&  vertices = mesh.impl->attrib.vertices;

		for (int i = 0; i < index.size(); i++)
		{
			int   in = index[i].vertex_index;
			Point p1(vertices[in * 3], vertices[in * 3 + 1], vertices[in * 3 + 2]);
			impl->bbox_.expandBy(p1 * matrix_);
		}
	}
}

//https://blog.csdn.net/xiaobaitu389/article/details/75523018

ShadeInfo MeshObject::intersect(const Ray& ray)
{
	ShadeInfo info;
	info.dis = FLT_MAX;

	if (impl->uesExtent || World::Instance()->accel_)
	{
		if (!impl->extent_.valid())
			this->init_polytope_boundingbox(impl->extent_);

		if (!impl->extent_.intersect(ray))
			return info;
	}
	else
	{
		if (!impl->bbox_.valid())
			this->computBBox();

		if (!impl->bbox_.intersect(ray))
		{
			return info;
		}
	}
	
	float t, u, v;
	const Mesh& mesh = impl->mesh_;

	for (tinyobj::shape_t& shape : impl->mesh_.impl->shapes)
	{
		std::vector<tinyobj::index_t>& index	= shape.mesh.indices;
		std::vector<tinyobj::real_t>&  vertices = mesh.impl->attrib.vertices;

		for (int i = 0; i < index.size(); i += 3)
		{
			int   in = index[i].vertex_index;
			Point p1(vertices[in * 3], vertices[in * 3 + 1], vertices[in * 3 + 2]);
			in = index[i + 1].vertex_index;
			Point p2(vertices[in * 3], vertices[in * 3 + 1], vertices[in * 3 + 2]);
			in = index[i + 2].vertex_index;
			Point p3(vertices[in * 3], vertices[in * 3 + 1], vertices[in * 3 + 2]);

			if (g::rayTriangleIntersect(ray, p1 * matrix_, p2 * matrix_, p3 * matrix_, t, u, v))
			{
				if (t < info.dis)
				{
					Vec3 v0v1 = p2 - p1;
					Vec3 v0v2 = p3 - p1;

					info.setShape(this);
					info.dis	  = t;
					info.u		  = u;
					info.v		  = v;
					info.position = ray.distance(t);
					//cout << info.position << "\t";

					info.ray	= ray;
					info.normal = v0v1 ^ v0v2;
					info.normal.normalize();
				}
			}
		}
	}

	return info;
}

void MeshObject::init_polytope_boundingbox(Extent& extent) const
{
	const Mesh& mesh = impl->mesh_;

	for (int j = 0; j < 7; j++)
	{
		for (tinyobj::shape_t& shape : mesh.impl->shapes)
		{
			std::vector<tinyobj::index_t>& index	= shape.mesh.indices;
			std::vector<tinyobj::real_t>&  vertices = mesh.impl->attrib.vertices;

			for (int i = 0; i < index.size(); i++)
			{
				int  in = index[i].vertex_index;
				Vec3 p1(vertices[in * 3], vertices[in * 3 + 1], vertices[in * 3 + 2]);
				p1 = p1 * matrix_;

				float d = g::planeSetNormals[j] * p1;
				// set dNEar and dFar
				if (d < extent.slabs_[j][0]) extent.slabs_[j][0] = d;
				if (d > extent.slabs_[j][1]) extent.slabs_[j][1] = d;
			}
		}
	}

	if(extent.mesh_ == this)
	   impl->extent_ = extent;
}
