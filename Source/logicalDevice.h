#pragma once

#include <Vulkan/vulkan.h>
#include <Vulkan/vulkan_core.h>
#include "device.h"

class LogicalDevice
{
public:
	LogicalDevice();
	virtual ~LogicalDevice();

public:
	void Initialize(Device& device);

private:
	VkPhysicalDevice _physicalDevice;
	uint32_t _deviceIndex = 0;
	VkQueue _queue;
};