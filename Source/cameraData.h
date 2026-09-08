//DB
#pragma once
#include <glm/glm.hpp>

struct CameraData
{
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::vec3 position;

	float nearPlane;
	float farPlane;
	float fov;
	float aspect;
};