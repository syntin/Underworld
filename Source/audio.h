#pragma once
#include <string>

struct Audio
{
	std::string soundName = ""; // name of sound file
	float volume = 1.0f;
	float pitch = 1.0f;
	bool loop = false;

	bool play = false;  // Script will set this to true
	bool dirty = true;
};