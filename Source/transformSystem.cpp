#include "transformSystem.h"
#include "componentManager.h"
#include "transformationMathematics.h"

void TransformSystem::Update(ComponentManager& components)
{
	auto& entities = components.GetTransformEntities();

	for (auto& entity : entities)
	{
		Transform* t = components.GetTransform(entity);
		if (!t) continue;

		if (!t->dirty)
			continue;

		t->localMatrix = CalculateLocalMatrix(*t);

		t->worldMatrix = t->localMatrix;

		t->dirty = false;
	}
}