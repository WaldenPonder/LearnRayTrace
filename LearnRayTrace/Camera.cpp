#include "stdafx.h"
#include "Camera.h"
#include "Ray.h"
#include "Vec3.h"
#include "Sphere.h"
#include "Plane.h"
#include "World.h"


Camera::Camera(World& world) : _world(world)
{

}

Camera::~Camera()
{
}

void Camera::render()
{
	float ratio = _width / (float)_height;
	float fov = tan(60. / 2 * PI / 180);

	std::ofstream OF(util::getDesktopPath() + "/out.ppm");
	
	OF << "P3\n" << _width << " " << _height << "\n255\n";
	   	
	for (float j = 0; j < _height; j+= 1)
	{
		for (float i = 0; i < _width; i+= 1)
		{

			int n = 5;
			Color c;// = _world._bgColor;

			for (int p = 0; p < n; p++)
			{
				for (int q = 0; q < n; q++)
				{
					float px = (2 * (i + .5) / _width - 1) * fov * ratio;
					float py = (1 - 2 * ((j + .5) / _height)) * fov;

					Vec3 dir = Vec3(px, py, -1) - Vec3(0.);
					dir.normalize();

					Ray ray(Vec3(0.), dir);

					c += _world.trace_ray(ray, 0);
				}
			}

			c /= (n * n);
			c *= 255;

			int ir = c[0];
			int ig = c[1];
			int ib = c[2];

			OF << ir << " " << ig << " " << ib << "\n";
		}
	}

	OF.close();
}