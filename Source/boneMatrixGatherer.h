//DB
#pragma once
#include "boneMatrixList.h"
#include "componentManager.h"

class BoneMatrixGatherer
{
public:
	void Gather(ComponentManager& components, BoneMatrixList& outList, float dt);
};