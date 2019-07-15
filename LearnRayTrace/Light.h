#pragma once
#include "Vec3.h"

class Light : public InstancePool<Light>, public Object
{
public:
	__class__(Light);

	Light();
	~Light();

	virtual Vec3 getDir(ShadeInfo& info) const = 0;
	virtual Vec3 L(ShadeInfo& info) const = 0;
};

//-------------------------------------------------PointLight
class PointLight : public Light
{
public:
	__class__(PointLight);
	PointLight();
	PointLight(const Point& pt, const Vec3& c);

	virtual Vec3 getDir(ShadeInfo& info) const override;
	virtual Vec3 L(ShadeInfo& info) const override;

	Point pt_;
	Vec3 color_ = g::White;
};

//-------------------------------------------------DirectionLight
class DirectionLight : public Light
{
public:
	__class__(DirectionLight);

	DirectionLight();
	DirectionLight(const Vec3& dir, const Vec3& c);

	virtual Vec3 getDir(ShadeInfo& info) const override;
	virtual Vec3 L(ShadeInfo& info) const override;

	Vec3 dir_;
	Vec3 color_ = g::White;
};