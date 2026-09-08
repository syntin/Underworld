#pragma once
#include "componentManager.h"

class EntityManger;

class TransformSystem
{
public:
	void Update(ComponentManager& components, SceneGraph& graph);
};