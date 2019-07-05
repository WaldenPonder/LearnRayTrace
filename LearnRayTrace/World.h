#pragma once
#include "Singleton.h"
#include "Camera.h"
#include <vector>

class Acceleration;
class World : public Singleton<World>
{
public:
	World();
	~World();

	void buildScene1();
	void buildScene2();

	ShadeInfo intersection(const Ray& ray) const;
	ShadeInfo intersection_without_meshobject(const Ray& ray) const;

	Color trace_ray(const Ray ray, int depth);
	Color trace_ray_direct(const Ray ray, int depth); //÷±Ω”π‚’’

	void max_to_one(Color& c) const;
	void clamp_to_color(Color& c, const Color& clamto = g::Red) const;

public:
	std::vector<Shape*> shapes_;
	Camera              camera_;
	int                 max_depth_;
	Color				bgColor_ = Color(58, 179, 255) / 255.f;
	Acceleration* accel_ = nullptr;
};

