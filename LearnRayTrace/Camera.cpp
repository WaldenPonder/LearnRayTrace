#include "stdafx.h"
#include "Camera.h"
#include "Matte.h"
#include "MultiJittered.h"
#include "PhotonMap.h"
#include "Plane.h"
#include "Ray.h"
#include "Setting.h"
#include "Sphere.h"
#include "Vec3.h"
#include "World.h"
#include <atomic>
#include <mutex>
#include <thread>
#include <cstdio>
#include <corecrt_io.h>

std::mutex g_cout_mutex;

long start_clock = clock();

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
	Camera*			   camera;
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
				std::lock_guard<mutex> guard(g_cout_mutex);
				pre_out = draw_pixel_count / total;
				cout << int(pre_out * 100) << "%"
					 << "\t" << std::this_thread::get_id() << endl;

				//if (int(pre_out * 100) == 30)
				//{
				//	camera->output_ppm("out_30%");
				//}
				//else if (int(pre_out * 100) == 70)
				//{
				//	camera->output_ppm("out_70%");
				//}
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

				Ray ray(Vec3(0.), dir, 0);

				if (Setting::Instance()->traceType == Setting::eDirect)
				{
					Vec3 f = World::Instance()->trace_ray_direct(ray);
					c += f / ( float )kSamples;
				}
				else if (Setting::Instance()->traceType == Setting::ePathTrace)
				{
					Vec3 f = World::Instance()->trace_ray(ray);
					c += f / ( float )kSamples;
				}
				else if (Setting::Instance()->traceType == Setting::ePhotonMap)
				{
					Vec3 f = World::Instance()->trace_photon(ray);
					c += f / ( float )kSamples;
				}
			}

			c[0] = pow(c[0], 1. / 2.2), c[1] = pow(c[1], 1. / 2.2), c[2] = pow(c[2], 1. / 2.2);
			World::Instance()->max_to_one(c);

			c.clamp(0, 1);
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
	impl->camera = this;
}

Camera::~Camera()
{
	delete impl;
}

void Camera::render()
{
	vector<std::shared_ptr<std::thread>> ths;

	for (size_t i = 0; i < std::thread::hardware_concurrency(); i++)
	{
		std::shared_ptr<std::thread> th = std::make_shared<std::thread>(&Camera::Impl::render_impl, impl, i);
		ths.push_back(th);
	}

	for (std::shared_ptr<std::thread>& th : ths)
	{
		th->join();
	}

	output_ppm("out");
	cout << "finish " << (clock() - start_clock) << endl;

	const char* path1 = std::string(g::getExeDir() + "/out_30%.ppm").c_str();
	const char* path2 = std::string(g::getExeDir() + "/out_70%.ppm").c_str();

	if (_access(path1, 0) != -1) remove(path1);
	if (_access(path2, 0) != -1) remove(path2);
}

void Camera::output_ppm(const std::string& filename)
{
	const int kSamples = Setting::Instance()->sample;

	std::ofstream OF(g::getExeDir() + "/" + filename + ".ppm");

	OF << "P3 # # \n"
	   << impl->width << " " << impl->height << " #\n255  # #  \n";
	OF << "#\t SAMPES\t" << kSamples << "\n";
	OF << "# HARDWARE_CONCURRENCY\t" << std::thread::hardware_concurrency() << "\n";

	OF << "#  TIME " << float(clock() - start_clock) / ( float )CLOCKS_PER_SEC << "\n";
	OF << "#  KDTREE_GET_POINT_COUNT  " << PhotonMap::Instance()->kdtree_get_point_count() << "\n";
	OF << "#  PHOTON_MAP_NEARST_SIZE " << PHOTON_MAP_NEARST_SIZE << "\n";

	for (size_t i = 0; i < impl->colors.size(); i++)
	{
		OF << impl->colors[i].x() << "\t" << impl->colors[i].y() << "\t" << impl->colors[i].z() << endl;
	}

	OF.close();
}
