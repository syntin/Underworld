#pragma once
#include "entity.h"

class ComponentManager;

class SkeletonSystem
{
public:
	void Update(ComponentManager& components);
};