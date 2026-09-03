#pragma once
#include "entity.h"

class ComponentManager;

class HealthSystem
{
public:
	void ApplyDamage(ComponentManager& components, Entity e, int amount);
	void ApplyHealing(ComponentManager& components, Entity e, int amount);
	void Update(ComponentManager& components);
};