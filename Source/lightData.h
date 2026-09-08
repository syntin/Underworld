//DB
#pragma once
#include <glm/glm.hpp>

struct LightData
{
	glm::vec3 position;	// world-space position
	glm::vec3 direction; // world-space direction for directional/spot

	glm::vec3 color;
	float intensity;

	float range; //point/spot
	float spotAngle; //spot only

	int type; // 0 = point, 1 = directional, 2 = spot
};