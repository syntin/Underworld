#pragma once

#include <vulkan/vulkan.h>

class BindlessRender
{
public:
	BindlessRender();
	virtual ~BindlessRender();

public:
	VkPhysicalDeviceDescriptorIndexingFeatures QueryDeviceForBindlessSupport();
	void CreateAndEnableBindlessDevice(VkPhysicalDeviceDescriptorIndexingFeatures indexingFeatures, VkDevice vulkanDevice);

private:
	bool _bindlessSupported = false;
	bool _bindlessEnabled = false;
	VkPhysicalDevice _vulkanPhysicalDevice;
};