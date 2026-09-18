// Author: RC

#pragma once

#include <cstdint>
#include <iostream>
#include "device.h"
#include "synchronization.h"
#include "window.h"
#include "utils.h"

class CommandBuffer
{
public:
	CommandBuffer();
	virtual ~CommandBuffer();
	/*
	VkCommandBuffer GetCommandBufferForFrame(uint32_t frameIndex)
	{
		return _syncro.GetFrameResources()[frameIndex]._commandBuffer;
	}
	*/
public:
	bool CreateCommandBuffers(
		Device device,
		Window window,
		uint32_t gfxQueueFamIdx,  
		std::array<FrameResources, MaxFramesInFlight>& frameResources);

private:
	//Synchronization _syncro{};
};

