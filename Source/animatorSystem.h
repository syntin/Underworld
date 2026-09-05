#pragma once
#include "entity.h"

class ComponentManager;

class AnimatorSystem
{
public:
	void Update(ComponentManager& components, float deltaTime);
};