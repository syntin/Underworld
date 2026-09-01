#include "cameraSystem.h"
#include "componentManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

void CameraSystem::Update(ComponentManager& components)
{
	auto& entities = components.GetCameraEntities();

	for (auto& entity : entities)
	{
		Camera* cam = components.GetCamera(entity);
		Transform* t = components.GetTransform(entity);

		if (!cam || !t)
			continue;

		// Only update if something changed
		if (!cam->dirty && !t->dirty)
			continue;

		// Compute forward vector from rotation
		float pitch = glm::radians(t->rotation.x);
		float yaw = glm::radians(t->rotation.y);

		glm::vec3 forward;
		forward.x = cos(yaw) * cos(pitch);
		forward.y = sin(pitch);
		forward.z = sin(yaw) * cos(pitch);

		glm::vec3 up = glm::vec3(0, 1, 0);

		// View matrix
		cam->viewMatrix = glm::lookAt(
			t->position,
			t->position + forward,
			up
		);

		// Projection matrix
		cam->projectionMatrix = glm::perspective(
			glm::radians(cam->fov),
			cam->aspect,
			cam->nearPlane,
			cam->farPlane
		);

		// Vulkan flips Y
		cam->projectionMatrix[1][1] *= -1;

		cam->dirty = false;
	}
}