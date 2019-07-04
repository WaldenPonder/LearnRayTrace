#include "stdafx.h"
#include "Cylinder.h"


Cylinder::Cylinder()
{
}


Cylinder::~Cylinder()
{
}

ShadeInfo Cylinder::intersect(const Ray& ray)
{
	float radus = .01f;

	ShadeInfo info;

	float dx = ray.dir.x();
	float dy = ray.dir.y();

	float ox = ray.orig.x();
	float oy = ray.orig.y();

	float A = dx * dx + dy * dy;
	float B = 2 * (dx * ox + dy * oy);
	float C = ox * ox + oy * oy - radus * radus;

	float delta = B * B - 4 * A * C;

	if (delta > 0)
	{
		float t1 = (-B + sqrt(delta)) / (2 * A);
		float t2 = (-B - sqrt(delta)) / (2 * A);

		info.dis = max(t1, t2);
		info.setShape(this);
		info.position = ray.distance(info.dis);
		info.normal = info.position;
		info.normal.normalize();
		info.ray = ray;
	}

	return info;
}
