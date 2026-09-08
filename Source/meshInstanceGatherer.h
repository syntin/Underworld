//DB
#pragma once
#include "renderList.h"
#include "entity.h"
#include "componentManager.h"

class MeshInstanceGatherer
{
public:
	void Gather(ComponentManager& components, RenderList& outList);
};