#pragma once
#include "Object.h"

class BTDF : public Object
{
public:
	__class__(BTDF);

	BTDF();
	~BTDF();

	virtual Vec3 f(const ShadeInfo& si, const Vec3& wo, const Vec3& wi) const;
	virtual Vec3 sample_f(const ShadeInfo& si, const Vec3& wo, Vec3& wt) const;
	
	virtual bool tir(const ShadeInfo& si) const; //total internal reflection


	float kt_;  
	float ior_; //’€…‰¬ 
};

