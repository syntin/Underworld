#pragma once
#include <string>
#include <unordered_map>
#include "entity.h"

struct Animator
{
	std::string currentState = "Idle"; // 
	std::string nextState = "";		   // for blending

	float blendTime = 0.0f;				//for how long blending lasts
	float blendTimer = 0.0f;			// current blend progress

	std::unordered_map<std::string, Entity> clips;
	// maps state name -> entity containing AnimationClip

	bool dirty = true;
};