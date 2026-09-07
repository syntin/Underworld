//DB
#include "boneMatrixGatherer.h"
#include "skeletalComponents.h"
#include "animationStateComponents.h"
#include "animationClip.h"
#include "animator.h"
#include "Transform.h"

static void EvaluateClipAtTIme(
	const AnimationClip& clip,
	Skeleton& skeleton,
	float time)
{
	// For each track in the clip, find the matching bone and compute its local transform
	for (const BoneTrack& track : clip.tracks)
	{
		//Find bone by name
		int boneIndex = -1;
		for (int i = 0; i < (int)skeleton.bones.size(); ++i)
		{
			if (skeleton.bones[i].name == track.boneName)
			{
				boneIndex = i;
				break;
			}
		}
		if (boneIndex < 0) continue;

		const BoneTrack& bt = track;
		if (bt.keyframes.empty()) continue;

		// Find two keyframes around 'time'
		const BoneKeyframe* k0 = &bt.keyframes.front();
		const BoneKeyframe* k1 = &bt.keyframes.back();

		for (int i = 0; i < (int)bt.keyframes.size() - 1; ++i)
		{
			if (time >= bt.keyframes[i].time && time <= bt.keyframes[i + 1].time)
			{
				k0 = &bt.keyframes[i];
				k1 = &bt.keyframes[i + 1];
				break;
			}
		}

		float t = 0.0f;
		float dt = k1->time - k0->time;
		if (dt > 0.0f)
			t = (time - k0->time) / dt;

		glm::vec3 pos = glm::mix(k0->position, k1->position, t);
		glm::quat rot = glm::slerp(k0->rotation, k1->rotation, t);
		glm::vec3 scl = glm::mix(k0->scale, k1->scale, t);

		glm::mat4 T = glm::translate(glm::mat4(1.0f), pos);
		glm::mat4 R = glm::toMat4(rot);
		glm::mat4 S = glm::scale(glm::mat4(1.0f), scl);

		skeleton.bones[boneIndex].localTransform = T * R * S;
	}

	//Compute world transforms (hierarchy)
	for (int i = 0; i < (int)skeleton.bones.size(); ++i)
	{
		Bone& bone = skeleton.bones[i];
		if (bone.parentIndex < 0)
		{
			bone.worldTransform = bone.localTransform;
		}
		else
		{
			Bone& parent = skeleton.bones[bone.parentIndex];
			bone.worldTransform = parent.worldTransform * bone.localTransform;
		}
	}

	// Compute skin matrices
	skeleton.skinMatrices.resize(skeleton.bones.size());
	for (int i = 0; i < (int)skeleton.bones.size(); ++i)
	{
		Bone& bone = skeleton.bones[i];
		skeleton.skinMatrices[i] = bone.worldTransform * bone.inverseBindMatrix;
	}
}

void BoneMatrixGatherer::Gather(ComponentManager& components, BoneMatrixList& outList, float dt)
{
	outList.Clear();

	auto& skeletonEntities = components.GetSkeletonEntities();

	for (Entity e : skeletonEntities)
	{
		Skeleton* skeleton = components.GetSkeleton(e);
		Animation* anim = components.GetAnimation(e);
		Animator* animator = components.GetAnimator(e);

		if (!skeleton || !anim || !animator)
			continue;

		// Advance animation time
		anim->frame += anim->speed * dt;
		float time = anim->frame / anim->speed; //seconds

		// Get clip entity from Animator
		auto it = animator->clips.find(anim->state);
		if (it == animator->clips.end())
			continue;

		Entity clipEntity = it->second;
		AnimationClip* clip = components.GetAnimationClip(clipEntity);
		if (!clip)
			continue;

		// Wrap time for looping
		float clipDuration = clip->duration;
		if (clipDuration > 0.0f && clip->loop)
		{
			while (time > clipDuration)
				time -= clipDuration;
		}

		// Evaluate clip into skeleton
		EvaluateClipAtTIme(*clip, *skeleton, time);

		// Push into boneMatrixList
		BoneMatrixEntry entry;
		entry.entity = e;
		entry.matrices = skeleton->skinMatrices;

		outList.entries.push_back(entry);
	}
}