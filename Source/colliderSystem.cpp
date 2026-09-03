#include "colliderSystem.h"
#include "componentManager.h"
#include "Transform.h"

static bool AABBvsAABB(const glm::vec3& aPos, const glm::vec3& aHalf,
					   const glm::vec3& bPos, const glm::vec3& bHalf)
{
	return  (fabs(aPos.x - bPos.x) <= (aHalf.x + bHalf.x)) &&
			(fabs(aPos.y - bPos.y) <= (aHalf.y + bHalf.y)) &&
			(fabs(aPos.z - bPos.z) <= (aHalf.z + bHalf.z));
}

static bool SphereVsSphere(const glm::vec3& aPos, float aRadius,
						   const glm::vec3& bPos, float bRadius)
{
	float dist = glm::distance(aPos, bPos);
	return dist <= (aRadius + bRadius);
}

bool ColliderSystem::CheckCollision(Entity a, Entity b, ComponentManager& components)
{
	Collider* ca = components.GetCollider(a);
	Collider* cb = components.GetCollider(b);
	if (!ca || !cb) return false;

	Transform* ta = components.GetTransform(a);
	Transform* tb = components.GetTransform(b);
	if (!ta || !tb) return false;

	glm::vec3 posA = ta->worldMatrix * glm::vec4(ca->offset, 1.0f);
	glm::vec3 posB = ta->worldMatrix * glm::vec4(cb->offset, 1.0f);

	if (ca->type == ColliderType::AABB && cb->type == ColliderType::AABB)
		return AABBvsAABB(posA, ca->halfExtents, posB, cb->halfExtents);

	if (ca->type == ColliderType::Sphere && cb->type == ColliderType::Sphere)
		return SphereVsSphere(posA, ca->radius, posB, cb->radius);

	// AABB vs Sphere
	if (ca->type == ColliderType::AABB && cb->type == ColliderType::Sphere)
	{
		glm::vec3 closest = glm::clamp(posB, posA - ca->halfExtents, posA + ca->halfExtents);
		float dist = glm::distance(closest, posB);
		return dist <= cb->radius;
	}

	if (ca->type == ColliderType::Sphere && cb->type == ColliderType::AABB)
	{
		glm::vec3 closest = glm::clamp(posA, posB - cb->halfExtents, posB + cb->halfExtents);
		float dist = glm::distance(closest, posA);
		return dist <= ca->radius;
	}

	return false;
}

void ColliderSystem::Update(ComponentManager& components)
{
	auto& entities = components.GetColliderEntities();

	for (size_t i = 0; i < entities.size(); ++i)
	{
		for (size_t j = i + 1; j < entities.size(); ++j)
		{
			Entity a = entities[i];
			Entity b = entities[j];

			if (CheckCollision(a, b, components))
			{
				//Future: trigger events
				// example: scriptSystem.OnCollision(a, b);
			}
		}
	}
}