//DB
#pragma once
#include "entity.h"
#include <glm/glm.hpp>

struct PhysicsContact
{
	Entity a;
	Entity b;

	glm::vec3 normal;		// collision normal (from A to B)
	float penetration;		// how deep the overlap is
};