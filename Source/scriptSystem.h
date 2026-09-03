#pragma once
#include "entity.h"

class ComponentManager;

class ScriptSystem
{
public:
	void Update(ComponentManager& components);
};