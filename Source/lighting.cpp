#include "lighting.h"
#include "componentManager.h"

// This is a work in progress, we don't have all of the stuff we need
// yet but i'll mark where we will eventually need to add stuff


void LightSystem::Update(ComponentManager& components)
{
	auto& entities = components.GetLightEntities();

	for (auto& e : entities)
	{
		Light* l = components.GetLight(e);
		if (!l) continue;

		if (!l->dirty)
			continue;

		// Future
		// Upload to GPU
		// Update uniform buffers
		// Update shadow maps
		// probably more stuff

		l->dirty = false;

	}
}