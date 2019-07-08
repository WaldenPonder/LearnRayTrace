#pragma once
#include "Shape.h"

class Camera
{
public:
	meta_name(Camera);

	Camera();
	~Camera();

	void render();

private:
	void  render_impl();

	struct Impl; 
	Impl * impl;
};

