//DB
#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "entity.h"

struct BoneMatrixEntry
{
	Entity entity; // skinned mesh entity
	std::vector<glm::mat4> matrices; // final skin matrices for entity
};

struct BoneMatrixList
{
	std::vector<BoneMatrixEntry> entries;

	void Clear()
	{
		entries.clear();
	}
};