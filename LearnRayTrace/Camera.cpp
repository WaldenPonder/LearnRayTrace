#include "stdafx.h"
#include "Camera.h"
#include "Ray.h"
#include "Vec3.h"
#include "Sphere.h"
#include "Plane.h"
#include "World.h"
#include "Matte.h"
#include "MultiJittered.h"

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

	int SIZE = 1;

	//std::cout << "输入采样数\n";
	//std::cin >> SIZE;

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
				//Point2D pt = MultiJittered::instance()->sample_unit_square();
				Point2D pt(.5);
				float px = (2 * (i + pt.x()) / _width - 1) * fov * ratio;
				float py = (1 - 2 * ((j + pt.y()) / _height)) * fov;

				Vec3 dir = Vec3(px, py, -1) - Vec3(0.);
				dir.normalize();

				Ray ray(Vec3(0.), dir);
				Color f = _world.trace_ray_direct(ray, 0);
				c = c + f / (float)SIZE;
			}

			_world.max_to_one(c);
			c.clamp(0, 1);

			c *= 255;
			int ir   = c[0];
			int ig   = c[1];
			int ib   = c[2];

			OF << ir << " " << ig << " " << ib << "\n";
		}
	}

	OF << "# " << float(clock() - t) / (float)CLOCKS_PER_SEC << "   sample\t" << SIZE;

	OF.close();

	cout << "finish " << (clock() - t) << endl;
}
