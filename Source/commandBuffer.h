#pragma once

#include <cstdint>
#include <iostream>
#include "device.h"
#include "synchronization.h"
#include "utils.h"

class CommandBuffer
{
public:
	CommandBuffer();
	virtual ~CommandBuffer();

public:
	bool CreateCommandBuffers(VkDevice device, uint32_t gfxQueueFamIdx, std::array<FrameResources, MaxFramesInFlight>& frameResources);

private:
	Synchronization _syncro{};
};

