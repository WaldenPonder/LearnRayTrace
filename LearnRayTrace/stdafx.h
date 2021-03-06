#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include "shlobj.h" 
#include <fstream>
#include "util.h"
#include "ShadeInfo.h"
#include "Ray.h"

begin_name_space(g);

const static Vec3 planeSetNormals[7] = { Vec3(1, 0, 0),
Vec3(0, 1, 0),
Vec3(0, 0, 1),
Vec3(std::sqrt(3.f) / 3.f, std::sqrt(3.f) / 3.f, std::sqrt(3.f) / 3.f),
Vec3(-std::sqrt(3.f) / 3.f, std::sqrt(3.f) / 3.f, std::sqrt(3.f) / 3.f),
Vec3(-std::sqrt(3.f) / 3.f, -std::sqrt(3.f) / 3.f, std::sqrt(3.f) / 3.f),
Vec3(std::sqrt(3.f) / 3.f, -std::sqrt(3.f) / 3.f, std::sqrt(3.f) / 3.f) };

end_name_space

#undef max
#undef min
using std::max;
using std::min;
using std::vector;