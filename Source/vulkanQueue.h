#pragma once

#include <vulkan/vulkan.h>
#include "device.h"

class VulkanQueue
{
public:
	VulkanQueue();
	virtual ~VulkanQueue();

public:
	void Initialize(VkInstance instance, Device& device);
	VkDeviceQueueCreateInfo GetCreateInfo() { return _queueCI; }

private:
	VkDeviceQueueCreateInfo _queueCI{};
	VkPhysicalDevice _physicalDevice;
	VkDevice _device;
};