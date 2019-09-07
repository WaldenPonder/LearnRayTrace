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
#include <atomic>
#include <mutex>
#include "Setting.h"

std::ofstream OF(g::getExeDir() + "/out.ppm");
std::mutex	cout_mutex;

struct Camera::Impl
{
	static const int width  = int(1920 * .8);
	static const int height = int(1080 * .8);
	const float		 ratio  = width / ( float )height;
	const float		 fov	= (60. / 2 * PI / 180);

	const float total = height * width;

	std::atomic<float> draw_pixel_count = 0;
	std::atomic<float> pre_out			= 0;
	vector<Vec3>	   colors;

	Impl()
	{
		colors.resize(width * height);
	}
	void render_impl(const int thread_index);
};

void Camera::Impl::render_impl(const int thread_index)
{
	const int kSamples = Setting::Instance()->sample;

	for (int j = 0; j < height; j += 1)
	{
		for (int i = 0; i < width; i += 1)
		{
			if ((i + j) % std::thread::hardware_concurrency() != thread_index)
				continue;

			draw_pixel_count = draw_pixel_count + 1;

			if (draw_pixel_count / total - pre_out > 0.01)
			{
				std::lock_guard<mutex> guard(cout_mutex);
				pre_out = draw_pixel_count / total;
				cout << int(pre_out * 100) << "%"
					 << "\t" << std::this_thread::get_id() << endl;
			}

			Vec3 c;

			for (int k = 0; k < kSamples; k++)
			{
				Point2D pt(.5);
				if (kSamples != 1)
					pt = MultiJittered::instance()->sample_unit_square();

				float px = (2 * (i + pt.x()) / width - 1) * fov * ratio;
				float py = (1 - 2 * ((j + pt.y()) / height)) * fov;

				Vec3 dir = Vec3(px, py, -1) - Vec3(0);
				dir.normalize();

				Ray ray(Vec3(0.), dir);

				if (Setting::Instance()->traceType == Setting::eDirect)
				{
					Vec3 f = World::Instance()->trace_ray_direct(ray, 0);
					c += f / ( float )kSamples;
				}
				else if (Setting::Instance()->traceType == Setting::ePathTrace)
				{
					Vec3 f = World::Instance()->trace_ray(ray, 0);
					c += f / ( float )kSamples;
				}
			}

			c.clamp(0, 1);

			c[0] = pow(c[0], 1. / 2.2), c[1] = pow(c[1], 1. / 2.2), c[2] = pow(c[2], 1. / 2.2);
			World::Instance()->max_to_one(c);
			
			c *= 255;

			int ir = c[0];
			int ig = c[1];
			int ib = c[2];

			int index	 = j * width + i;
			colors[index] = c;
		}
	}
}

Camera::Camera() : impl(new Impl)
{
}

Camera::~Camera()
{
	delete impl;
}

void Camera::render()
{
	long	  t		   = clock();
	const int kSamples = Setting::Instance()->sample;

	OF << "P3 # # \n"
	   << impl->width << " " << impl->height << " #\n255  # #  \n";
	OF << "#\t SAMPES\t" << kSamples << "\n";
	OF << "# HARDWARE_CONCURRENCY\t" << std::thread::hardware_concurrency() << "\n";

	MultiJittered::instance()->sample_hemisphere();

	vector< std::shared_ptr<std::thread> > ths;

	for (int i = 0; i < std::thread::hardware_concurrency(); i++)
	{
		std::shared_ptr<std::thread> th = std::make_shared<std::thread>(&Camera::Impl::render_impl, impl, i);
		ths.push_back(th);
	}

	for (std::shared_ptr<std::thread>& th : ths)
	{
		th->join();
	}

	for (int i = 0; i < impl->colors.size(); i++)
	{
		OF << impl->colors[i].x() << "\t" << impl->colors[i].y() << "\t" << impl->colors[i].z() << endl;
	}

	OF << "# " << float(clock() - t) / ( float )CLOCKS_PER_SEC << "   sample\t" << kSamples;
	OF.close();

	cout << "finish " << (clock() - t) << endl;
}