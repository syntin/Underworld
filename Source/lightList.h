//DB
#pragma once
#include "lightData.h"
#include <vector>

struct LightList
{
	std::vector<LightData> lights;

	void Clear()
	{
		lights.clear();
	}
};