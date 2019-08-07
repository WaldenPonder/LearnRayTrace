#include "stdafx.h"
#include "util.h"
#include <io.h>

begin_name_space(g);

string getDesktopPath()
{
	LPITEMIDLIST pidl;
	LPMALLOC	 pShellMalloc;
	char		 szDir[200];
	if (SUCCEEDED(SHGetMalloc(&pShellMalloc)))
	{
		if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl)))
		{
			// 如果成功返回true
			SHGetPathFromIDListA(pidl, szDir);
			pShellMalloc->Free(pidl);
		}
		pShellMalloc->Release();
	}

	return string(szDir);
}

std::string getExeDir()
{
	char* str = 0;
	_get_pgmptr(&str);
	string ss(str);
	ss = ss.substr(0, ss.find_last_of("\\") + 1);
	return ss;
}

std::string getResDir()
{
	string s = getExeDir() + "res/";
	if (!_access(s.c_str(), 0))
		return s;
	return getExeDir() + "../../res/";
}

//https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection

bool rayTriangleIntersect(
	const Ray&   ray,
	const Point& v0, const Point& v1, const Point& v2,
	float& t, float& u, float& v)
{
	Vec3  v0v1 = v1 - v0;
	Vec3  v0v2 = v2 - v0;
	Vec3  pvec = ray.dir ^ (v0v2);
	float det  = v0v1 * pvec;

	// if the determinant is negative the triangle is backfacing
	// if the determinant is close to 0, the ray misses the triangle
	if (fabs(det) < kEpsilon) return false;

	float invDet = 1 / det;

	Vec3 tvec = ray.orig - v0;
	u		  = tvec * ( pvec )*invDet;
	if (u < 0 || u > 1) return false;

	Vec3 qvec = tvec ^ (v0v1);
	v		  = ray.dir * ( qvec )*invDet;
	if (v < 0 || u + v > 1) return false;

	t = v0v2 *  qvec *invDet;

	return true;
}

Vec3 random_in_unit_sphere()
{
	Vec3 p;
	do {
		p = 2.0*Vec3(drand48(), drand48(), drand48()) - Vec3(1, 1, 1);
	} while (p * p >= 1.0);
	return p;
}

end_name_space
