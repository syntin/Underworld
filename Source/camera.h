#pragma once
#include <glm/glm.hpp>

struct Camera
{
	float fov = 60.0f;
	float nearPlane = 0.1f;
	float farPlane = 1000.0f;
	float aspect = 16.0f / 9.0f;

	glm::mat4 viewMatrix = glm::mat4(1.0f);
	glm::mat4 projectionMatrix = glm::mat4(1.0f);

	bool dirty = true;
};