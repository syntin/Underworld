#include "skeletalSystem.h"
#include "componentManager.h"

void SkeletonSystem::Update(ComponentManager& components)
{
	auto& entities = components.GetSkeletonEntities();

	for (auto& e : entities)
	{
		Skeleton* skel = components.GetSkeleton(e);
		if (!skel) continue;

		// Recompute world transforms
		for (size_t i = 0; i < skel->bones.size(); i++)
		{
			Bone& b = skel->bones[i];

			if (b.parentIndex == -1)
			{
				b.worldTransform = b.localTransform;
			}
			else
			{
				Bone& parent = skel->bones[b.parentIndex];
				b.worldTransform = parent.worldTransform * b.localTransform;
			}
		}

		// Compute skinning matrices
		skel->skinMatrices.resize(skel->bones.size());
		for (size_t i = 0; i < skel->bones.size(); i++)
		{
			skel->skinMatrices[i] = skel->bones[i].worldTransform * skel->bones[i].inverseBindMatrix;
		}
	}
}