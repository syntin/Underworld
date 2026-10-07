//DB

#include "physicsBroadphaseSystem.h"
#include "componentManager.h"
#include "culling.h"
#include "Transform.h"

static bool AABBOVerlap(const BoundsComponent& a, const BoundsComponent& b)
{
	return (a.worldBounds.min.x <= b.worldBounds.max.x &&
			a.worldBounds.max.x >= b.worldBounds.min.x &&
			a.worldBounds.min.y <= b.worldBounds.max.y &&
			a.worldBounds.max.y >= b.worldBounds.min.y &&
			a.worldBounds.min.z <= b.worldBounds.max.z &&
			a.worldBounds.max.z >= b.worldBounds.min.z);
}

void PhysicsBroadphaseSystem::Update(World& world)
{
	auto& cm = world.GetComponentManager();
	auto& entities = cm.GetEntitiesWithBounds();

	pairs.clear();

	const size_t count = entities.size();
	for (size_t i = 0; i < count; i++)
	{
		Entity a = entities[i];
		BoundsComponent* ba = cm.GetBounds(a);
		if (!ba) continue;

		for (size_t j = i + 1; j < count; ++j)
		{
			Entity b = entities[j];
			BoundsComponent* bb = cm.GetBounds(b);
			if (!bb) continue;

			if (AABBOVerlap(*ba, *bb))
			{
				pairs.push_back({ a,b });
			}
		}
	}
}