#include "animationSystem.h"
#include "componentManager.h"

void AnimationSystem::Update(ComponentManager& components, float deltaTime)
{
	auto& entities = components.GetAnimationEntities();

	for (auto& e : entities)
	{
		Animation* anim = components.GetAnimation(e);
		if (!anim) continue;

		// Advance frame
		anim->frame += anim->speed * deltaTime;

		// Loop or clamp
		if (anim->frame >= anim->frameCount)
		{
			if (anim->loop)
				anim->frame = fmod(anim->frame, anim->frameCount);
			else
				anim->frame = anim->frameCount - 1;
		}

		anim->dirty = false;
	}
}