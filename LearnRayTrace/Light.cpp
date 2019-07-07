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
Vec3 DirectionLight::getDir(ShadeInfo& info) const
{
	return dir_;
}

Vec3 DirectionLight::L(ShadeInfo& info) const
{
	return color_;
}
