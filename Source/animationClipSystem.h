#pragma once
#include "entity.h"

class ComponentManager;

class AnimationClipSystem
{
public:
	void Update(ComponentManager& components, float deltaTime);
};