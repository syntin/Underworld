//DB
#include "animationSystem.h"
#include "componentManager.h"

void AnimationSystem::Update(ComponentManager& components, float dt)
{
	auto& entities = components.GetAnimationEntities();

	for (Entity e : entities)
	{
		Animation* anim = components.GetAnimation(e);
		Animator* animator = components.GetAnimator(e);

		if (!anim || !animator)
			continue;

		// Find current clip
		auto it = animator->clips.find(anim->state);
		if (it == animator->clips.end())
			continue;

		AnimationClip* currentClip = components.GetAnimationClip(it->second);
		if (!currentClip)
			continue;

		// Advance current animation time
		anim->time += dt * anim->speed;

		// Looping
		if (currentClip->loop)
		{
			if (anim->time > currentClip->duration)
				anim->time = fmod(anim->time, currentClip->duration);
		}
		else
		{
			if (anim->time > currentClip->duration)
				anim->time = currentClip->duration;
		}

		// Blending logic
		if (!animator->nextState.empty())
		{
			animator->blendTimer += dt;

			if (animator->blendTimer >= animator->blendTime)
			{
				// Blending finished - switch to next state
				anim->state = animator->nextState;
				animator->nextState.clear();
				animator->blendTimer = 0.0f;

				// Reset animation time for new state
				anim->time = 0.0f;
			}
		}

		anim->dirty = true; // boneMatrixGatherer will handle blending
	}
}