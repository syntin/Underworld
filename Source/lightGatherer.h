//DB
#pragma once
#include "lightList.h"
#include "componentManager.h"

class LightGatherer
{
public:
	void Gather(ComponentManager& components, LightList& outList);
};