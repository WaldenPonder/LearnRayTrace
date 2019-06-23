#include "stdafx.h"
#include "Camera.h"
#include "Ray.h"
#include "Vec3.h"
#include "Sphere.h"
#include "Plane.h"
#include "World.h"
#include "Matte.h"

Camera::Camera(World& world) : _world(world)
{
	_width *= .8;
	_height *= .8;
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

//#pragma omp parallel for schedule(dynamic, 1)       // OpenMP

	int SIZE = 100;

	std::cout << "输入采样数\n";
	std::cin >> SIZE;

	OF << "#\t SAMPES\t" << SIZE << "\n";

	long t = clock();

	for (int j = 0; j < _height; j+= 1)
	{
		for (int i = 0; i < _width; i+= 1)
		{
			count++;

			if (count / total - pre_out > 0.01)
			{
				pre_out = count / total;
				cout << int(pre_out * 100) << "%" << endl;
			}

			Color c;
		
			for(int k = 0; k < SIZE; k++)
			{
				float px = (2 * (i + .5) / _width - 1) * fov * ratio;
				float py = (1 - 2 * ((j + .5) / _height)) * fov;

				Vec3 dir = Vec3(px, py, -1) - Vec3(0.);
				dir.normalize();

				Ray ray(Vec3(0.), dir);
				Color f = _world.trace_ray(ray, 0);
				c = c + f / (float)SIZE;
			}

			c *= 255;
			int ir   = c[0];
			int ig   = c[1];
			int ib   = c[2];

			OF << ir << " " << ig << " " << ib << "\n";
		}
	}

	OF << "# " << (clock() - t) / CLOCKS_PER_SEC << "   sample\t" << SIZE;

	OF.close();

	cout << "finish " << (clock() - t) << endl;
}
