#include "stdafx.h"
#include "Extent.h"
#include "MeshObject.h"

Extent::Extent()
{
	init();
}

Extent::Extent(MeshObject* mesh) : mesh_(mesh)
{
	init();
	mesh->init_polytope_boundingbox(*this);
}

Extent::Extent(const Extent& extent) : mesh_(extent.mesh_)
{
	for (int i = 0; i < 7; i++)
	{
		slabs_[i][0] = extent.slabs_[i][0];
		slabs_[i][1] = extent.slabs_[i][1];
	}
}

void Extent::operator=(const Extent& extent)
{
	mesh_ = extent.mesh_;
	for (int i = 0; i < 7; i++)
	{
		slabs_[i][0] = extent.slabs_[i][0];
		slabs_[i][1] = extent.slabs_[i][1];
	}
}

Extent::~Extent() {}

bool Extent::is_intersect(const Ray& ray)
{
	uint8_t planeIndex;

	float tNear = 0, tFar = FLT_MAX;  // tNear, tFar for the intersected extents

	for (uint8_t i = 0; i < 7; ++i)
	{
		float tNearExtents = (slabs_[i][0] - ray.precomputedNumerator[i]) / ray.precomputedDenominator[i];
		float tFarExtents  = (slabs_[i][1] - ray.precomputedNumerator[i]) / ray.precomputedDenominator[i];
		if (ray.precomputedDenominator[i] < 0)
			std::swap(tNearExtents, tFarExtents);
		if (tNearExtents > tNear)
			tNear = tNearExtents, planeIndex = i;
		if (tFarExtents < tFar)
			tFar = tFarExtents;
		if (tNear > tFar)
		{
			//cout << "AAA\n";
			return false;
		}
			
	}

	return true;
}

bool Extent::intersect(const float* precomputedNumerator, const float* precomputedDenominator,
					   float&   tNear,  // tn and tf in this method need to be contained
					   float&   tFar,   // within the range [tNear:tFar]
					   uint8_t& planeIndex) const
{
	for (uint8_t i = 0; i < 7; ++i)
	{
		float tNearExtents = (slabs_[i][0] - precomputedNumerator[i]) / precomputedDenominator[i];
		float tFarExtents  = (slabs_[i][1] - precomputedNumerator[i]) / precomputedDenominator[i];
		if (precomputedDenominator[i] < 0) std::swap(tNearExtents, tFarExtents);
		if (tNearExtents > tNear) tNear = tNearExtents, planeIndex = i;
		if (tFarExtents < tFar) tFar = tFarExtents;
		if (tNear > tFar)
		{
			//cout << "BBB\n";
			return false;
		}
	}

	return true;
}

Vec3 Extent::center() const
{
	return Vec3(slabs_[0][0] + slabs_[0][1] * 0.5, slabs_[1][0] + slabs_[1][1] * 0.5, slabs_[2][0] + slabs_[2][1] * 0.5);
}

void Extent::extendBy(const Extent& extent)
{
	for (uint8_t i = 0; i < 7; ++i)
	{
		if (extent.slabs_[i][0] < slabs_[i][0])
			slabs_[i][0] = extent.slabs_[i][0];
		if (extent.slabs_[i][1] > slabs_[i][1])
			slabs_[i][1] = extent.slabs_[i][1];
	}
}

void Extent::init()
{
	for (int i = 0; i < 7; i++)
	{
		slabs_[i][0] = FLT_MAX;
		slabs_[i][1] = -FLT_MAX;
	}
}

bool Extent::valid() const
{
	for (int i = 0; i < 7; i++)
	{
		if (g::equivalent(slabs_[i][0], FLT_MAX))
			return false;

		if (g::equivalent(slabs_[i][0], -FLT_MAX))
			return false;
	}

	return true;
}
