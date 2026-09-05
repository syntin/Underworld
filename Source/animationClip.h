#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>
#include <string>

struct BoneKeyframe
{
	float time; //seconds
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
};

struct BoneTrack
{
	std::string boneName;
	std::vector<BoneKeyframe> keyframes;
};

struct AnimationClip
{
	std::string name;  // animation name
	float duration = 0.0f; // total time in seconds
	float frameRate = 30.0f; // fps

	std::vector<BoneTrack> tracks;

	bool loop = true;
};