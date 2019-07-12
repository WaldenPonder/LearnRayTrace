#pragma once
#include "Ray.h"

//  #define  USING_COUT

class ShadeInfo
{
 public:
	meta_name(ShadeInfo);
	ShadeInfo();

	float  dis   = FLT_MAX;
	Shape* shape = nullptr;
	int	depth = 0;
	Vec3   normal;
	Vec3   hit_pos;

	Vec3  color;
	Ray   ray;
	float u = 0;
	float v = 0;

	Material* material = nullptr;

	enum eISINSHADOW
	{
		YES,
		NO,
		NOT_CALCULAT_YET
	};
	eISINSHADOW isInShadow = NOT_CALCULAT_YET;

	Vec3 reflect() const;

	void setShape(Shape* s);

	bool valid();
};
