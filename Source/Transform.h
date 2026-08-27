#pragma once
#include <cmath>

// We can change this out later if we use different vector/quat stuff
// I just wanted to make this as simple as possible for right now

struct Vec3
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

struct Quat
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;
};

struct Transform
{
	Vec3 position{ 0.0f, 0.0f, 0.0f };
	Quat rotation{ 0.0f, 0.0f, 0.0f, 1.0f };
	Vec3 scale{ 1.0f, 1.0f, 1.0f };
};