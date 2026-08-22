#pragma once

#include <vulkan/vulkan.h>

class BindlessRender
{
public:
	BindlessRender();
	virtual ~BindlessRender();

public:
	void SetAllocationCallbacks(const VkAllocationCallbacks* pCallbacks);
	VkPhysicalDeviceDescriptorIndexingFeatures QueryDeviceForBindlessSupport();
	void CreateAndEnableBindlessDevice(VkPhysicalDeviceDescriptorIndexingFeatures indexingFeatures, VkDevice vulkanDevice);

private:
	bool _bindlessSupported = false;
	bool _bindlessEnabled = false;
	VkPhysicalDevice _vulkanPhysicalDevice;
	const VkAllocationCallbacks* _vkAllocationCallbacks;
};