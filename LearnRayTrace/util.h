#pragma once
#include "Ray.h"

begin_name_space(g);

string getDesktopPath();
//Î²²¿²»´ø "\\"
string getExeDir();

bool rayTriangleIntersect(const Ray& ray, const Point& v0, const Point& v1, const Point& v2, float& t, float& u, float& v);

end_name_space;