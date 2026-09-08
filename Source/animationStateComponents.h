//DB 
#pragma once
#include <string>

struct Animation
{
	std::string state = "Idle"; // curent animation/state
	float time = 0.0f;		// current playback time in seconds
	float speed = 1.0f;		// playback speed multiplier
	bool loop = true;	

	Entity clipEntity = INVALID_ENTITY;

	bool dirty = true;

};