//DB
#include "animatorSystem.h"
#include "componentManager.h"

void AnimatorSystem::Update(ComponentManager& components, float dt)
{
    auto& entities = components.GetAnimatorEntities();

    for (Entity e : entities)
    {
        Animator* animator = components.GetAnimator(e);
        Animation* anim = components.GetAnimation(e);

        if (!animator || !anim)
            continue;

        // If nextState is set, begin blending
        if (!animator->nextState.empty())
        {
            animator->blendTimer += dt;

            if (animator->blendTimer >= animator->blendTime)
            {
                // Finish blend: switch animation state
                anim->state = animator->nextState;
                animator->nextState.clear();
                animator->blendTimer = 0.0f;

                // Reset animation time for new state
                anim->time = 0.0f;
            }

            animator->dirty = true;
        }

        auto it = animator->clips.find(anim->state);
        if (it == animator->clips.end())
            continue;

    }
}