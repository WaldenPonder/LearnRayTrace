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

	void buildScene1();
	void buildScene2();

	ShadeInfo intersection(const Ray& ray);
	Color trace_ray(const Ray ray, int depth);
	Color trace_ray_direct(const Ray ray, int depth); //÷±Ω”π‚’’

	void max_to_one(Color& c) const;
	void clamp_to_color(Color& c, const Color& clamto = g::Red) const;

public:
	std::vector<Shape*> _shapes;
	Camera              _camera;
	int                 _max_depth;
	Color				_bgColor = Color(58, 179, 255) / 255.f;
};

