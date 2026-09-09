//DB
#include "boneMatrixGatherer.h"
#include "skeletalComponents.h"
#include "animationStateComponents.h"
#include "animationClip.h"
#include "animator.h"
#include "Transform.h"

#include <glm/gtx/matrix_decompose.hpp>

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

		// Find current clip
		auto it = animator->clips.find(anim->state);
		if (it == animator->clips.end())
			continue;

		AnimationClip* currentClip = components.GetAnimationClip(it->second);
		if (!currentClip)
			continue;

		// Evaluate current clip
		Skeleton tempCurrent = *skeleton; 
		EvaluateClipAtTIme(*currentClip, tempCurrent, anim->time);

		bool blending = !animator->nextState.empty();

		Skeleton tempNext;
		float blendFactor = 0.0f;

		if (blending)
		{
			// Find next clip
			auto it2 = animator->clips.find(animator->nextState);
			if (it2 != animator->clips.end())
			{
				AnimationClip* nextClip = components.GetAnimationClip(it2->second);
				if (nextClip)
				{
					// Compute blend factor
					blendFactor = animator->blendTimer / animator->blendTime;
					blendFactor = glm::clamp(blendFactor, 0.0f, 1.0f);

					// Evaluate next clip at time 0 (start of transition)
					tempNext = *skeleton;
					EvaluateClipAtTIme(*nextClip, tempNext, 0.0f);
				}
			}
		}

		// Blend bones
		skeleton->skinMatrices.resize(skeleton->bones.size());

		for (int i = 0; i < (int)skeleton->bones.size(); ++i)
		{
			glm::mat4 currentMat = tempCurrent.skinMatrices[i];

			if (blending)
			{	// Decompose -> Interpolate -> Recompose 
				glm::mat4 nextMat = tempNext.skinMatrices[i];

				glm::vec3 posA, scaleA, skewA;
				glm::quat rotA;
				glm::vec4 perspA;

				glm::vec3 posB, scaleB, skewB;
				glm::quat rotB;
				glm::vec4 perspB;

				// Decompose current
				glm::decompose(currentMat, scaleA, rotA, posA, skewA, perspA);

				// Decompose next
				glm::decompose(nextMat, scaleB, rotB, posB, skewB, perspB);

				// Interpolate components
				glm::vec3 pos = glm::mix(posA, posB, blendFactor);
				glm::quat rot = glm::slerp(rotA, rotB, blendFactor);
				glm::vec3 scale = glm::mix(scaleA, scaleB, blendFactor);

				// Recompose
				glm::mat4 blended =
					glm::translate(glm::mat4(1.0f), pos) *
					glm::mat4_cast(rot) *
					glm::scale(glm::mat4(1.0f), scale);

				skeleton->skinMatrices[i] = blended;

			}
			else
			{
				skeleton->skinMatrices[i] = currentMat;
			}
		}

		// Output entry
		BoneMatrixEntry entry;
		entry.entity = e;
		entry.matrices = skeleton->skinMatrices;

		outList.entries.push_back(entry);

	}
}