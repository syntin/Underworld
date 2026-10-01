//DB
#pragma once
#include <glm/glm.hpp>

struct MovementComponent
{
	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 acceleration = glm::vec3(0.0f);
	float damping = 0.98f;
};