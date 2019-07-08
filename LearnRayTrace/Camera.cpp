#include "stdafx.h"
#include "Camera.h"
#include "Ray.h"
#include "Vec3.h"
#include "Sphere.h"
#include "Plane.h"
#include "World.h"
#include "Matte.h"
#include "MultiJittered.h"
#include <thread>

struct Camera::Impl
{
	std::ofstream OF;

	int width_  = int(1920 * .8);
	int height_ = int(1080 * .8);

	float ratio_;
	float fov_;

	float total			   = height_ * width_;
	float draw_pixel_count = 0;
	float pre_out		   = 0;

	int SAMPLE_SIZE = 5;
	int i			= 0;
	int j			= 0;
};

Camera::Camera() : impl(new Impl)
{
	impl->OF.open(g::getDesktopPath() + "/out.ppm");
}

Camera::~Camera()
{
}

void Camera::render()
{
	long t = clock();

	impl->ratio_ = impl->width_ / ( float )impl->height_;
	impl->fov_   = tan(60. / 2 * PI / 180);

	impl->OF << "P3\n"
			 << impl->width_ << " " << impl->height_ << "\n255\n";

	std::cout << "输入采样数\n";
	std::cin >> impl->SAMPLE_SIZE;

	impl->OF << "#\t SAMPES\t" << impl->SAMPLE_SIZE << "\n";

	vector<std::thread*> ths;

	for (int i = 0; i < 1; i++)
	{
		std::thread* th = new std::thread(&Camera::render_impl, this);
		ths.push_back(th);
	}

	for (std::thread* th : ths)
	{
		th->join();
	}

	impl->OF << "# " << float(clock() - t) / ( float )CLOCKS_PER_SEC << "   sample\t" << impl->SAMPLE_SIZE;
	impl->OF.close();

	cout << "finish " << (clock() - t) << endl;
}

void Camera::render_impl()
{
	for (; impl->j < impl->height_; impl->j += 1)
	{
		for (; impl->i < impl->width_; impl->i += 1)
		{
			impl->draw_pixel_count++;

			if (impl->draw_pixel_count / impl->total - impl->pre_out > 0.01)
			{
				impl->pre_out = impl->draw_pixel_count / impl->total;
				cout << int(impl->pre_out * 100) << "%" << endl;
			}

			Vec3 c;

			for (int k = 0; k < impl->SAMPLE_SIZE; k++)
			{
				//Point2D pt = MultiJittered::instance()->sample_unit_square();
				Point2D pt(.5);
				float   px = (2 * (impl->i + pt.x()) / impl->width_ - 1) * impl->fov_ * impl->ratio_;
				float   py = (1 - 2 * ((impl->j + pt.y()) / impl->height_)) * impl->fov_;

				Vec3 dir = Vec3(px, py, -1) - Vec3(0);
				dir.normalize();

				Ray  ray(Vec3(0.), dir);
				Vec3 f = g::Red;// World::Instance()->trace_ray_direct(ray, 0);
				c	  = c + f / ( float )impl->SAMPLE_SIZE;
			}

			World::Instance()->max_to_one(c);
			c.clamp(0, 1);

			c *= 255;
			int ir = c[0];
			int ig = c[1];
			int ib = c[2];

			impl->OF << ir << " " << ig << " " << ib << "\n";
		}
	}
}
