//DB
#pragma once
#include "cameraData.h"
#include "componentManager.h"
#include "entity.h"

class CameraGatherer
{
public:
	void Gather(ComponentManager& components, CameraData& outData);
};