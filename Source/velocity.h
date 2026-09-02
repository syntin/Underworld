#pragma once
#include <glm/glm.hpp>

struct Velocity
{
	glm::vec3 linear = glm::vec3(0.0f);
	glm::vec3 angular = glm::vec3(0.0f);

	bool dirty = true;
};