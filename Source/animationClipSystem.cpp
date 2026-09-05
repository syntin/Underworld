#include "animationClipSystem.h"
#include "componentManager.h"
#include <glm/gtx/quaternion.hpp>

static BoneKeyframe Interpolate(const BoneKeyframe& a, const BoneKeyframe& b, float t)
{
	BoneKeyframe result;
	result.time = t;

	float alpha = (t - a.time) / (b.time - a.time);

	result.position = glm::mix(a.position, b.position, alpha);
	result.rotation = glm::slerp(a.rotation, b.rotation, alpha);
	result.scale = glm::mix(a.scale, b.scale, alpha);

	return result;
}

void AnimationClipSystem::Update(ComponentManager& components, float deltaTime)
{
	auto& entities = components.GetAnimationClipEntities();

	for (auto& e : entities)
	{
		AnimationClip* clip = components.GetAnimationClip(e);
		Animation* anim = components.GetAnimation(e);
		Skeleton* skel = components.GetSkeleton(e);

		if (!clip || !anim || !skel)
			continue;

		// Advance animation time
		anim->frame += anim->speed * deltaTime;

		float currentTime = anim->frame / anim->speed;

		if (currentTime > clip->duration)
		{
			if (clip->loop)
				currentTime = fmod(currentTime, clip->duration);
			else
				currentTime = clip->duration;
		}

		// Evaluate each bone track
		for (auto& track : clip->tracks)
		{
			// find bone index
			int boneIndex = -1;
			for (int i = 0; i < skel->bones.size(); i++)
			{
				if (skel->bones[i].name == track.boneName)
				{
					boneIndex = i;
					break;
				}
			}
			if (boneIndex == -1) continue;

			// find keyframes around current time
			const auto& keys = track.keyframes;
			if (keys.empty()) continue;

			BoneKeyframe k0 = keys.front();
			BoneKeyframe k1 = keys.back();

			for (int i = 0; i < keys.size() - 1; i++)
			{
				if (currentTime >= keys[i].time && currentTime <= keys[i + 1].time)
				{
					k0 = keys[i];
					k1 = keys[i + 1];
					break;
				}
			}

			BoneKeyframe result = Interpolate(k0, k1, currentTime);

			// write into skeleton
			Bone& bone = skel->bones[boneIndex];
			bone.localTransform =
				glm::translate(glm::mat4(1.0f), result.position) *
				glm::toMat4(result.rotation) *
				glm::scale(glm::mat4(1.0f), result.scale);

			skel->dirty = true;

		}

	}

}