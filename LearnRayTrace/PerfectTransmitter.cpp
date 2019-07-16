#include "stdafx.h"
#include "PerfectTransmitter.h"


PerfectTransmitter::PerfectTransmitter()
{
}


PerfectTransmitter::~PerfectTransmitter()
{
}

Vec3 PerfectTransmitter::f(const ShadeInfo& si, const Vec3& wo, const Vec3& wi) const
{
	return Vec3();
}

Vec3 PerfectTransmitter::sample_f(const ShadeInfo& si, const Vec3& wo, Vec3& wt) const
{
	Vec3 n(si.normal);
	float cos_thetai = n * wo;

	float eta = ior_;

	if(cos_thetai < .0)
	{
		cos_thetai = -cos_thetai;
		n = -n;
		eta = 1 / eta;
	}

	float tmp = 1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta);
	float cos_theta2 = std::sqrt(tmp);

	wt = -wo / eta - (cos_theta2 - cos_thetai / eta) * n;
	wt.normalize();

	return kt_ / (eta * eta) * g::White / fabs(si.normal * wt);
}

bool PerfectTransmitter::tir(const ShadeInfo& si) const
{
	Vec3 wo(-si.ray.dir);
	float cos_thetai = si.normal * wo;

	float eta = ior_;

	if (cos_thetai < 0.0)
		eta = 1 / eta;

	return 1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta) < 0.0;
}
