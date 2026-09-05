#pragma once
#include <string>

struct Animation
{
	std::string state = "Idle"; // curent animation/state
	float frame = 0.0f;			// current frame
	float frameCount = 1.0f;	// total frames in the animation
	float speed = 1.0f;			// fps
	bool loop = true;			

	bool dirty = true;
};