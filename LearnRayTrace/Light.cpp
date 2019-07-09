#include "stdafx.h"
#include "Light.h"


Light::Light()
{
}


Light::~Light()
{
}

Vec3 Light::getDir(ShadeInfo& info) const
{	
	return Vec3();
}

Vec3 Light::L(ShadeInfo& info) const
{
	return Vec3();
}

//-------------------------------------------------PointLight

PointLight::PointLight(const Point& pt, const Vec3& c)
{
	pt_ = pt;
	color_ = c;
}

PointLight::PointLight()
{

}

Vec3 PointLight::getDir(ShadeInfo& info) const
{
	Vec3 dir = pt_ - info.position;
	dir.normalize();
	return dir;
}

Vec3 PointLight::L(ShadeInfo& info) const
{
	return color_;
}

//-------------------------------------------------DirectionLight

DirectionLight::DirectionLight(const Vec3& dir, const Vec3& c)
{
	dir_ = dir;
	dir_.normalize();
	color_ = c;
}

DirectionLight::DirectionLight()
{

}


Vec3 DirectionLight::getDir(ShadeInfo& info) const
{
	return dir_;
}

Vec3 DirectionLight::L(ShadeInfo& info) const
{
	return color_;
}
