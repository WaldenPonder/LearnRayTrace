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
	ShadeInfo intersection(const Ray& ray);
	Color trace_ray(const Ray ray, int depth);

public:
	std::vector<Shape*> _shapes;
	Camera              _camera;
	int                 _max_depth;
	Color				_bgColor = Color(58, 179, 255) / 255.f;
};

