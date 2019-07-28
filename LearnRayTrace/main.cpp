#include "stdafx.h"
#include "World.h"
#include "BVH.h"
#include "Setting.h"

int main()
{
	if(0)
	{
		string path = g::getExeDir() + "setting.xml";
#if 0
		std::ofstream os(path);
		cereal::XMLOutputArchive ar(os);
		ar(*Setting::Instance());
#else
		std::ifstream os(path);
		cereal::XMLInputArchive ar(os);
		ar(*Setting::Instance());
#endif
	}

	//World::Instance()->accel_ = new BVH;
	World::Instance()->camera_.render();

	system("pause");
    return 0;
}

