#pragma once
#include "Material.h"

class Checker : public Material
{
public:
	Checker(const Vec3& c1 = g::Black, const Vec3& c2 = g::White, float kr = .75);
	~Checker();

	virtual Vec3 shade(ShadeInfo& r) override;
	virtual Vec3 shade_direct(ShadeInfo& r) override;
	
	float kr_; //·´ÉäÏµÊý
	Vec3 c1_;
	Vec3 c2_;
};

