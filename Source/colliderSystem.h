#pragma once
#include "entity.h"

class ComponentManager;

class ColliderSystem
{
public:
	void Update(ComponentManager& components);

	bool CheckCollision(Entity a, Entity b, ComponentManager& components);
};