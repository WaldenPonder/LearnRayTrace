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

std::ofstream OF(g::getDesktopPath() + "/out.ppm");
std::mutex	of_mutex;
//#define  multi_thread

struct Camera::Impl
{
	static const int width  = int(1920 * .8);
	static const int height = int(1080 * .8);
	static const int SAMPLE_SIZE = 1;
	const float ratio = width / ( float )height;
	const float fov   = (60. / 2 * PI / 180);

	const float total = height * width;

	std::atomic<float> draw_pixel_count = 0;
	std::atomic<float> pre_out			= 0;
	std::atomic<int> i			 = 0;
	std::atomic<int> j			 = 0;
	std::atomic<int> k			 = 0;

	std::atomic<bool> flag[width][height][5];	
	std::mutex	cout_mutex;

	void  render_impl();
};

void Camera::Impl::render_impl()
{
	for (; j < height; j += 1)
	{
		if (i >= width) i = 0;

		for (; i < width; i += 1)
		{
			draw_pixel_count = draw_pixel_count + 1;

			if (draw_pixel_count / total - pre_out > 0.01)
			{
#ifdef multi_thread
				std::lock_guard<std::mutex>	guard(cout_mutex);
#endif
				pre_out = draw_pixel_count / total;
				cout << int(pre_out * 100) << "%" << "\t" << std::this_thread::get_id() << endl;
			}

			Vec3 c;
			if (k >= SAMPLE_SIZE) k = 0;
	
			for (; k < SAMPLE_SIZE; k++)
			{
				if (flag[i][j][k])
					continue;

				flag[i][j][k] = true;

				Point2D pt(.5);
				if(SAMPLE_SIZE != 1)
					pt = MultiJittered::instance()->sample_unit_square();

				float   px = (2 * (i + pt.x()) / width - 1) * fov * ratio;
				float   py = (1 - 2 * ((j + pt.y()) / height)) * fov;

				Vec3 dir = Vec3(px, py, -1) - Vec3(0);
				dir.normalize();

				Ray  ray(Vec3(0.), dir);
				Vec3 f = World::Instance()->trace_ray_direct(ray, 0);
				c = c + f / (float)SAMPLE_SIZE;
			}

			World::Instance()->max_to_one(c);
			c.clamp(0, 1);

			c *= 255;
			int ir = c[0];
			int ig = c[1];
			int ib = c[2];

			{
				
#ifdef multi_thread
				std::lock_guard<mutex> guard(of_mutex);
#endif
				OF << ir << " " << ig << " " << ib << "\n";
			}
		}
	}
}

Camera::Camera() : impl(new Impl)
{

}

Camera::~Camera()
{
}

void Camera::render()
{
	long t = clock();

	OF << "P3\n" << impl->width << " " << impl->height << "\n255\n";
	OF << "#\t SAMPES\t" << impl->SAMPLE_SIZE << "\n";
	OF << "# HARDWARE_CONCURRENCY\t" << std::thread::hardware_concurrency() << "\n";

	for (int i = 0; i < impl->width; i++)
	{
		for (int j = 0; j < impl->height; j++)
		{
			for (int k = 0; k < impl->SAMPLE_SIZE; k++)
			{
				impl->flag[i][j][k] = false;
			}
		}
	}
	MultiJittered::instance()->sample_hemisphere();

#ifdef multi_thread
	vector<std::thread*> ths;

	for (int i = 0; i < std::thread::hardware_concurrency(); i++)
	{
		std::thread* th = new std::thread(&Camera::Impl::render_impl, impl);
		ths.push_back(th);
	}

	for (std::thread* th : ths)
	{
		th->join();
	}
#else
	impl->render_impl();
#endif
	
	OF << "# " << float(clock() - t) / ( float )CLOCKS_PER_SEC << "   sample\t" << impl->SAMPLE_SIZE;
	OF.close();

	cout << "finish " << (clock() - t) << endl;
}