#pragma once
#include "BTDF.h"

class PerfectTransmitter : public BTDF
{
 public:
	 __class__(PerfectTransmitter);

	PerfectTransmitter();
	virtual ~PerfectTransmitter();

	virtual Vec3 f(const ShadeInfo& si, const Vec3& wo, const Vec3& wi) const override;
	virtual Vec3 sample_f(const ShadeInfo& si, const Vec3& wo, Vec3& wt) const override;

	virtual bool tir(const ShadeInfo& si) const override; //total internal reflection
};
