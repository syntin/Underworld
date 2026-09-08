#pragma once
#include "componentManager.h"
#include "SceneGraph.h"

class EntityManger;

class TransformSystem
{
public:
	void Update(ComponentManager& components, SceneGraph& graph);
};