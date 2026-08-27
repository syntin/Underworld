#pragma once
#include <vulkan/vulkan.h>
#include "extensions.h"
#include "device.h"

class VulkanInstance
{
public:
	VulkanInstance();
	virtual ~VulkanInstance();

public:
	void Initialize();
	void DestroyInstance();
	VkInstance GetInstance() { return _instance; }

private:
	bool CheckValidationLayerSupport();

private:
	VkInstance _instance;
	Extensions _extensions;
	Device _device;
};