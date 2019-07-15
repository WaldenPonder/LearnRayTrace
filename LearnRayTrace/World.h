#pragma once
#include "Singleton.h"
#include "Camera.h"

class World : public Singleton<World>, public Object
{
public:
	__class__(World);
	World();
	~World();

	void buildScene();

	ShadeInfo intersection(const Ray& ray) const;
	ShadeInfo intersection_without_meshobject(const Ray& ray) const;

	Vec3 trace_ray(const Ray ray, int depth);
	Vec3 trace_ray_direct(const Ray ray, int depth); //÷±Ω”π‚’’

	void max_to_one(Vec3& c) const;
	void clamp_to_color(Vec3& c, const Vec3& clamto = g::Red) const;

	Vec3 get_ambient() const;

	bool isInShadow(ShadeInfo& info) const;
	
public:
	Camera              camera_;
	int                 max_depth_;
	Vec3				bgColor_ = Vec3(58, 179, 255) / 255.f;
	Acceleration* accel_ = nullptr;
};

