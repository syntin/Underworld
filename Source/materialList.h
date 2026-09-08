//DB
#pragma once
#include "material.h"
#include <vector>

struct MaterialList
{
	std::vector<Material*> materials;

	void Clear()
	{
		materials.clear();
	}
};