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

        // Advance animation time
        anim->time += dt * anim->speed;

        // Find current clip
        auto it = animator->clips.find(anim->state);
        if (it == animator->clips.end())
            continue;

        AnimationClip* currentClip = components.GetAnimationClip(it->second);
        if (!currentClip)
            continue;

        // Looping / clamping
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

        // Mark dirty so BoneMatrixGatherer recomputes
        anim->dirty = true;
    }
}