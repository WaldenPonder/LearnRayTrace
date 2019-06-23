#include "stdafx.h"
#include "MeshObject.h"
#include "tiny_obj_loader.h"



struct MeshObject::Impl
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	bool triangulate = true;
	string filename;
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
}

ShadeInfo MeshObject::intersect(const Ray& ray)
{
	return ShadeInfo();
}