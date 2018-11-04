#pragma once
#include "Shape.h"
#include <vector>
#include "Singleton.h"
#include "Camera.h"

class World : public Singleton<World>
{
public:
	World();
	~World();

	void buildScene();
	ShadeInfo intersection(Ray& ray);

public:
	std::vector<Shape*> _shapes;
	Camera              _camera;
};

