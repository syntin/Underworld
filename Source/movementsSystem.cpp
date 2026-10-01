//DB
#include "movementSystem.h"
#include "World.h"
#include "componentManager.h"
#include "Transform.h"
#include "movementComponent.h"

void MovementSystem::Update(World& world)
{
	float dt = world.GetDeltaTime();
	auto& cm = world.GetComponentManager();

	// Get all entities that have MovementComponent
	auto& entities = cm.GetMovementEntities();

	for (Entity e : entities)
	{
		MovementComponent* move = cm.GetMovement(e);
		Transform* transform = cm.GetTransform(e);

		if (!move || !transform)
			continue;

		move->velocity += move->acceleration * dt;

		move->velocity *= move->damping;

		transform->position += move->velocity * dt;
	}
}