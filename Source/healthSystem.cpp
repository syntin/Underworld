#include "healthSystem.h"
#include "componentManager.h"

void HealthSystem::ApplyDamage(ComponentManager& components, Entity e, int amount)
{
	Health* h = components.GetHealth(e);
	if (!h) return;

	h->current -= amount;
	if (h->current < 0)
		h->current = 0;

	h->dirty = true;
}

void HealthSystem::ApplyHealing(ComponentManager& components, Entity e, int amount)
{
	Health* h = components.GetHealth(e);
	if (!h) return;

	h->current += amount;
	if (h->current > h->max)
		h->current = h->max;

	h->dirty = true;
}

void HealthSystem::Update(ComponentManager& components)
{
	auto& entities = components.GetHealthEntities();

	for (auto& e : entities)
	{
		Health* h = components.GetHealth(e);
		if (!h) continue;

		if (!h->dirty)
			continue;

		//Future: trigger ui updates, scripts, events or whatever else we want
		// Example:
		//  if (h->current == 0) { OnDeath(e); }

		h->dirty = false;

	}
}

// for future, we don't need it now but later we will
// void HealthSystem::OnDeath(Entity e)
// {
//		// remove components
//      // trigger any scripts or events tied to death
//		// play animation 
//  }


