#include "stdafx.h"
#include "Setting.h"

Setting::Setting()
{
	sample	= 1;
	max_depth = 5;
	//traceType = eDirect;
	//traceType = ePathTrace;
	traceType = ePhotonMap;
}

Setting::~Setting()
{
}
