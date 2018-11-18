#pragma once
#include "Shape.h"

class World;

class Camera
{
public:
	Camera(World& world);
	~Camera();

public:
	void  render();

public:
	int   _width   = int(1920 * .8);
	int   _height  = int(1080 * .8);
		
	World& _world;
	Vec3  _eye;
	Vec3  _front;
	Vec3  _up;
	Vec3  _right;
	float _fovy;
	float _fovScale;
};

