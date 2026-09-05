#include "animatorSystem.h"
#include "componentManager.h"

void AnimatorSystem::Update(ComponentManager& components, float deltaTime)
{
	auto& entities = components.GetAnimatorEntities();

	for (auto& e : entities)
	{
		Animator* animator = components.GetAnimator(e);
		Animation* anim = components.GetAnimation(e);

		if (!animator || !anim)
			continue;

		// if nextState is set, begin blending
		if (!animator->nextState.empty())
		{
			animator->blendTimer += deltaTime;

			if (animator->blendTimer >= animator->blendTime)
			{
				// Finish blend
				animator->currentState = animator->nextState;
				animator->nextState = "";
				animator->blendTimer = 0.0f;
			}

			animator->dirty = true;
		}

		// Pick correct clip entity
		auto it = animator->clips.find(animator->currentState);
		if (it == animator->clips.end())
			continue;

		Entity clipEntity = it->second;

		// Attach the clip to the animationComponent
		anim->clipEntity = clipEntity;
	}
}