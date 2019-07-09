#pragma once
#include "Shape.h"

class Camera
{
public:
	meta_name(Camera);

	Camera();
	~Camera();

	void render();

	struct Impl; 
	Impl * impl;
};

