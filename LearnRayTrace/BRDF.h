#pragma once
#include "Vec3.h"
#include "ShadeInfo.h"

class Sampler;

//√Ë ˆæµ√Ê∑¥…‰
class BRDF
{
public:
	BRDF();
	~BRDF();

	virtual Color sample_f(const ShadeInfo& si, const Vec3& wo, Vec3& wi, float& pdf) const;

	Sampler* _sampler = nullptr;
	Material* _material = nullptr;
};

