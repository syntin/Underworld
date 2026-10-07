//DB
#pragma once
#include <glm/glm.hpp>

struct RigidBodyComponent
{
	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 acceleration = glm::vec3(0.0f);

	float mass = 1.0f;
	float restitution = 0.1f; // bounciness
	float friction = 0.8f;

	bool isStatic = false; //walls, floors
};