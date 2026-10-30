//DB
#include "aiSystem.h"
#include "World.h"
#include "componentManager.h"

#include <glm/glm.hpp>

void AISystem::Update(World& world)
{
	auto& cm = world.GetComponentManager();

	Entity player = world.GetPlayerEntity();

	Transform* playerTransform =
		cm.GetTransform(player);

	if (!playerTransform)
		return;

	for (Entity e : cm.GetAIEntities())
	{
		AIComponent* ai =
			cm.GetAI(e);

		Transform* transform =
			cm.GetTransform(e);

		MovementComponent* move =
			cm.GetMovement(e);

		if (!ai || !transform || !move)
			continue;

		glm::vec3 toPlayer =
			playerTransform->position -
			transform->position;

		float distance =
			glm::length(toPlayer);

		if (distance < ai->detectionRange)
		{
			ai->state = AIState::Chase;
		}
		else
		{
			ai->state = AIState::Idle;
		}

		if (ai->state == AIState::Chase)
		{
			glm::vec3 direction =
				glm::normalize(toPlayer);

			move->acceleration =
				direction *
				ai->moveSpeed;
		}

	}
}