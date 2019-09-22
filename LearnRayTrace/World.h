#pragma once
#include "Camera.h"
#include "common.inc"   // for Red, __class__
#include "Object.h"     // for Object
#include "ShadeInfo.h"  // for ShadeInfo
#include "Vec3.h"       // for Vec3
#include "Singleton.h"
class Acceleration;
class Ray;

class World : public Singleton<World>, public Object
{
public:
	__class__(World);
	World();
	~World();

	void buildScene();
	void build_photon_map(const int photon_num);

	ShadeInfo intersection(const Ray& ray) const;
	ShadeInfo intersection_without_meshobject(const Ray& ray) const;

	Vec3 trace_ray(const Ray ray);
	Vec3 trace_ray_direct(const Ray ray); //÷±Ω”π‚’’
	Vec3 trace_photon(const Ray ray);

	void collect_photon(const Ray ray, Vec3 c);

	void max_to_one(Vec3& c) const;
	void clamp_to_color(Vec3& c, const Vec3& clamto = g::Red) const;

	Vec3 get_ambient() const;

	bool isInShadow(ShadeInfo& info) const;

public:
	Camera              camera_;
	Vec3				bgColor_ = Vec3(58, 179, 255) / 255.f;
	Acceleration* accel_ = nullptr;
	bool is_collect_photon_ = false;
};

