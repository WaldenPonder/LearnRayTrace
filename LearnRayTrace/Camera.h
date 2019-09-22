#pragma once
#include "Shape.h"
#include <string>

class Camera : public Object
{
public:
	__class__(Camera);

	Camera();
	~Camera();

	void render();
	void output_ppm(const std::string& filename);

	struct Impl; 
	Impl * impl;
};

