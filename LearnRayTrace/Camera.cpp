#include "stdafx.h"
#include "Camera.h"
#include "Ray.h"
#include "Vec3.h"
#include "Sphere.h"
#include "Plane.h"
#include "World.h"
#include "Phong.h"

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
	
	float total = _height * _width;
	float count = 0;
	float pre_out = 0;

	for (float j = 0; j < _height; j+= 1)
	{
		for (float i = 0; i < _width; i+= 1)
		{
			count++;

			if (count / total - pre_out > 0.01)
			{
				pre_out = count / total;
				cout << int(pre_out * 100) << "%" << endl;
			}

			int n = 10;
			Color c;
		
			{
				float px = (2 * (i + .5) / _width - 1) * fov * ratio;
				float py = (1 - 2 * ((j + .5) / _height)) * fov;

				Vec3 dir = Vec3(px, py, -1) - Vec3(0.);
				dir.normalize();

				Ray ray(Vec3(0.), dir);

				ShadeInfo info(_world.intersection(ray));

				if (info.valid())
				{
					c = info.material->shade(info);
				}
			}

			c *= 255;
			int ir   = c[0];
			int ig   = c[1];
			int ib   = c[2];

			OF << ir << " " << ig << " " << ib << "\n";
		}
	}

	OF.close();
}
