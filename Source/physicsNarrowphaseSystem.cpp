//DB
#include "physicsNarrowphaseSystem.h"
#include "World.h"
#include "componentManager.h"
#include "culling.h"
#include <algorithm>

static bool AABBOverlap(const AABB& a, const AABB& b)
{
	return (a.min.x <= b.max.x && a.max.x >= b.min.x &&
			a.min.y <= b.max.y && a.max.y >= b.min.y &&
			a.min.z <= b.max.z && a.max.z >= b.min.z);
}

static PhysicsContact ComputeContact(Entity a, const AABB& A, Entity b, const AABB& B)
{
	// Compute overlap on each axis
	float overlapX = std::min(A.max.x, B.max.x) - std::max(A.min.x, B.min.x);
	float overlapY = std::min(A.max.y, B.max.y) - std::max(A.min.y, B.min.y);
	float overlapZ = std::min(A.max.z, B.max.z) - std::max(A.min.z, B.min.z);

	// Find smallest penetration axis
	float penetration = overlapX;
	glm::vec3 normal(1, 0, 0);

	if (overlapY < penetration)
	{
		penetration = overlapY;
		normal = glm::vec3(0, 1, 0);
	}

	if (overlapZ < penetration)
	{
		penetration = overlapZ;
		normal = glm::vec3(0, 0, 1);
	}

	// Determine direction of normal (from A to B)
	glm::vec3 centerA = (A.min + A.max) * 0.5f;
	glm::vec3 centerB = (B.min + B.max) * 0.5f;

	glm::vec3 dir = centerB - centerA;
	if (glm::dot(dir, normal) < 0)
		normal = -normal;

	PhysicsContact contact;
	contact.a = a;
	contact.b = b;
	contact.normal = normal;
	contact.penetration = penetration;

	return contact;
}

void PhysicsNarrowphaseSystem::Update(World& world)
{
	contacts.clear();

	auto& cm = world.GetComponentManager();
	auto& pairs = world.GetBroadphaseSystem()->GetPairs();

	for (auto& pair : pairs)
	{
		BoundsComponent* ba = cm.GetBounds(pair.a);
		BoundsComponent* bb = cm.GetBounds(pair.b);

		if (!ba || !bb)
			continue;

		if (AABBOverlap(ba->worldBounds, bb->worldBounds))
		{
			contacts.push_back(
				ComputeContact(pair.a, ba->worldBounds,
					pair.b, bb->worldBounds)
			);
		}
	}
}