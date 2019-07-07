#pragma once
#include "Vec3.h"

class Light : public InstancePool<Light>
{
public:
	meta_name(Light);

	Light();
	~Light();

	virtual Vec3 getDir(ShadeInfo& info) const = 0;
	virtual Vec3 L(ShadeInfo& info) const = 0;
};

//-------------------------------------------------PointLight
class PointLight : public Light
{
public:
	meta_name(PointLight);

	virtual Vec3 getDir(ShadeInfo& info) const override;
	virtual Vec3 L(ShadeInfo& info) const override;

	Point pt_;
	Vec3 color_ = g::White;
};

//-------------------------------------------------DirectionLight
class DirectionLight : public Light
{
public:
	meta_name(DirectionLight);

	virtual Vec3 getDir(ShadeInfo& info) const override;
	virtual Vec3 L(ShadeInfo& info) const override;

	Vec3 dir_;
	Vec3 color_ = g::White;
};