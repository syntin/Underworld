// Author: RC

#pragma once

#include <volk/volk.h>

class BindlessRender
{
public:
	BindlessRender();
	virtual ~BindlessRender();

public:
	VkPhysicalDeviceDescriptorIndexingFeatures QueryDeviceForBindlessSupport();
	void CreateAndEnableBindlessDevice(VkPhysicalDeviceDescriptorIndexingFeatures indexingFeatures, VkDevice vulkanDevice);
	void Render();

private:
	bool _bindlessSupported = false;
	bool _bindlessEnabled = false;
	VkPhysicalDevice _vulkanPhysicalDevice;
};