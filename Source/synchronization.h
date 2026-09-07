#pragma once
#include <vulkan/vulkan.h>
#include "device.h"
#include "window.h"

struct FrameResources
{
	VkCommandPool _commandPool = nullptr;
	VkCommandBuffer _commandBuffer = nullptr;
	VkSemaphore _imageAcquiredSemaphore = nullptr;
};


class Synchronization
{
public:
	Synchronization();
	virtual ~Synchronization();

public:
	bool Initialize(Device device, Window window);
	VkSemaphore GetTimelineSemaphore() { return _timelineSemaphore; }
	std::array<FrameResources, MaxFramesInFlight> GetFrameResources() { return _frameResources; }

protected:
	bool CreateSyncResources(Device device, Window window);

private:
	VkSemaphore _timelineSemaphore = nullptr;
	std::array<FrameResources, MaxFramesInFlight> _frameResources;
};

