// Author: RC

#pragma once

#include <volk/volk.h>
#include "device.h"
#include "surface.h"

class GraphicsQueue
{
public:
	GraphicsQueue();
	virtual ~GraphicsQueue();

public:
	void Create(VkInstance& instance, Device device);
	bool FindGraphicsQueue(Surface* surface, Device* device);
	uint32_t GetGraphicsQueueFamilyIndex() { return _gfxQueueFamIdx; }
	VkQueue* GetGraphicsQueue() { return &_gfxQueue; }

private:
	VkPhysicalDevice _physicalDevice = nullptr;
	VkDevice _device = nullptr;
	uint32_t _gfxQueueFamIdx = 0;
	VkQueue _gfxQueue = nullptr;
};