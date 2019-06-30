#include "stdafx.h"
#include "MeshObject.h"
#include "tiny_obj_loader.h"
#include "util.h"
#include "BoundingBox.h"

struct MeshObject::Impl
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	bool triangulate = true;
	string filename;
	BoundingBox bbox;
};

MeshObject::MeshObject(const string& filename) : impl(new Impl)
{
	impl->filename = filename;
	load();
}


MeshObject::~MeshObject()
{
}

void MeshObject::load()
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

void MeshObject::computBBox()
{
	for (tinyobj::shape_t& shape : impl->shapes)
	{
		std::vector<tinyobj::index_t>& index = shape.mesh.indices;
		std::vector<tinyobj::real_t>& vertices = impl->attrib.vertices;

		for (int i = 0; i < index.size(); i++)
		{
			int in = index[i].vertex_index;
			Point p1(vertices[in * 3], vertices[in * 3 + 1], vertices[in * 3 + 2]);
			impl->bbox.expandBy(p1* magnify + offset);
		}
	}
}

//https://blog.csdn.net/xiaobaitu389/article/details/75523018

ShadeInfo MeshObject::intersect(const Ray& ray)
{
	ShadeInfo info;
	info.distance = FLT_MAX;

	if (!impl->bbox.valid())
		this->computBBox();

	if (!impl->bbox.intersect(ray))
	{
		return info;
	}
	
	float t, u, v;
	
	for (tinyobj::shape_t& shape : impl->shapes)
	{
		std::vector<tinyobj::index_t>& index = shape.mesh.indices;
		std::vector<tinyobj::real_t>& vertices = impl->attrib.vertices;

		for (int i = 0; i < index.size(); i += 3)
		{
			int in = index[i].vertex_index;
			Point p1(vertices[in * 3], vertices[in * 3 + 1], vertices[in * 3 + 2]);
			in = index[i + 1].vertex_index;
			Point p2(vertices[in * 3], vertices[in * 3 + 1], vertices[in * 3 + 2]);
			in = index[i + 2].vertex_index;
			Point p3(vertices[in * 3], vertices[in * 3 + 1], vertices[in * 3 + 2]);

			if (util::rayTriangleIntersect(ray, p1 * magnify + offset, p2* magnify + offset, p3* magnify + offset, t, u, v))
			{
				if (t < info.distance)
				{
					Vec3 v0v1 = p2 - p1;
					Vec3 v0v2 = p3 - p1;

					info.setShape(this);
					info.distance = t;
					info.u = u;
					info.v = v;
					info.position = ray.distance(t);
					//cout << info.position << "\t";

					info.ray = ray;
					info.normal = v0v1 ^ v0v2;
					info.normal.normalize();
				}
			}
		}
	}
	
	return info;
}