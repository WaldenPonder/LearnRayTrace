#pragma once
#include "Ray.h"

begin_name_space(g);

string getDesktopPath();
//Î²²¿´ø "\\"
string getExeDir();

string getResDir();

bool rayTriangleIntersect(const Ray& ray, const Point& v0, const Point& v1, const Point& v2, float& t, float& u, float& v);

inline Vec3 random_cosine_direction()
{
	float r1  = drand48();
	float r2  = drand48();
	float z   = sqrt(1 - r2);
	float phi = 2 * PI * r1;
	float x   = cos(phi) * sqrt(r2);
	float y   = sin(phi) * sqrt(r2);
	return Vec3(x, y, z);
}

inline Vec3 random_to_sphere(float radius, float distance_squared)
{
	float r1  = drand48();
	float r2  = drand48();
	float z   = 1 + r2 * (sqrt(1 - radius * radius / distance_squared) - 1);
	float phi = 2 * PI * r1;
	float x   = cos(phi) * sqrt(1 - z * z);
	float y   = sin(phi) * sqrt(1 - z * z);
	return Vec3(x, y, z);
}

Vec3 random_in_unit_sphere();

end_name_space;