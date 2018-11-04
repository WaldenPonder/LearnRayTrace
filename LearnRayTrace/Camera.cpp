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
	float fov = tan(60. / 2 * util::PI / 180);

	std::ofstream OF(util::getDesktopPath() + "/out.ppm");
	
	OF << "P3\n" << _width << " " << _height << "\n255\n";
	   	
	for (float j = 0; j < _height; j+= 1)
	{

		for (float i = 0; i < _width; i+= 1)
		{
			float px = (2 * (i + .5) / _width - 1) * fov * ratio;
			float py = (1 - 2 * ((j + .5) / _height)) * fov;
			
			//cout << px << ", " << py << endl;

			Vec3 dir = Vec3(px, py, -1) - Vec3(0.);
			dir.normalize();

			Ray ray(Vec3(0.), dir);

			Color c = _bgColor;

			ShadeInfo r = _world.intersection(ray);
			if (r)
			{
				c = r.color;
			}

			int ir = c[0];
			int ig = c[1];
			int ib = c[2];

			OF << ir << " " << ig << " " << ib << "\n";
		}
	}

	OF.close();
}