#include "stdafx.h"
#include "World.h"
#include "BVH.h"
/*

###先别关注加速，先把效果做好

多线程
grid加速
*/

int main()
{
	//World::Instance()->accel_ = new BVH;
	World::Instance()->camera_.render();

	system("pause");
    return 0;
}

