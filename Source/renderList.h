//DB
#pragma once
#include "renderInstance.h"
#include <vector>

struct RenderList
{
	std::vector<RenderInstance> instances;

	void Clear()
	{
		instances.clear();
	}
};