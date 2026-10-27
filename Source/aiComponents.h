//DB

#pragma once

enum class AIState
{
	Idle,
	Chase
};

struct AIComponent
{
	AIState state = AIState::Idle;

	float detectionRange = 5.0f;
	float moveSpeed = 2.0f;
};