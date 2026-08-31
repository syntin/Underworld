#include "Transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

glm::mat4 CalculateLocalMatrix(const Transform& t)
{
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), t.position);

	glm::mat4 rotation =
		glm::yawPitchRoll(t.rotation.y, t.rotation.x, t.rotation.z);

	glm::mat4 scaling = glm::scale(glm::mat4(1.0f), t.scale);

	return translation * rotation * scaling;
}