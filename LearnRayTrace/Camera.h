#pragma once
#include "Shape.h"

class Camera : public Object
{
public:
	__class__(Camera);

	Camera();
	~Camera();

	void render();

	struct Impl; 
	Impl * impl;
};

