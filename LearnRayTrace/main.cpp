#include "stdafx.h"
#include "World.h"
#include "BVH.h"
/*
阴影
镜面反射

###先别关注加速，先把效果做好
多线程
grid加速

extent模式，求阴影求不到???
*/

int main()
{
	//World::Instance()->accel_ = new BVH;
	World::Instance()->camera_.render();

	system("pause");
    return 0;
}

