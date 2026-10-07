#include "transformSystem.h"
#include "componentManager.h"
#include "transformationMathematics.h"
#include "SceneGraph.h"
#include "culling.h"

void TransformSystem::Update(ComponentManager& components, SceneGraph& graph)
{
	auto& entities = components.GetTransformEntities();

	for (auto& entity : entities)
	{
		Transform* t = components.GetTransform(entity);
		if (!t) continue;

		bool hierarchDirty = graph.IsDirty(entity);

		// Skip if nothing changed
		if (!t->dirty && !hierarchDirty)
			continue;

		// Recompute local matrix if needed
		if (t->dirty)
			t->localMatrix = CalculateLocalMatrix(*t);

		// Combine with parent world matrix if parent exists
		Entity parent = graph.GetParent(entity);
		if (parent.index != 0 && components.HasTransform(parent))
		{
			Transform* pt = components.GetTransform(parent);
			t->worldMatrix = pt->worldMatrix * t->localMatrix;
		}
		else
		{
			t->worldMatrix = t->localMatrix;
		}

		if (components.HasBounds(entity))
		{
			BoundsComponent* bc = components.GetBounds(entity);
			bc->worldBounds = TransformAABB(bc->localBounds, t->worldMatrix);
		}

		//clear dirty flags
		t->dirty = false;
		graph.ClearDirty(entity);
	}
}