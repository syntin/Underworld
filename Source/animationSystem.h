//DB
#pragma once
#include "entity.h"

class ComponentManager;

class AnimationSystem
{
public:
	void Update(ComponentManager& components, float deltaTime);
};