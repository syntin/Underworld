// Author: RC

#pragma once

#include <volk/volk.h>
#include "extensions.h"
#include "device.h"

class VulkanInstance
{
public:
	VulkanInstance();
	virtual ~VulkanInstance();

public:
	bool Create();
	void Destroy();

public:
	VkInstance& GetInstance() { return _vulkanInstance; }
	VkInstance* Data() { return &_vulkanInstance; }
	void SetInstance(VkInstance instance) { _vulkanInstance = instance; }

protected:
	bool CheckValidationLayerSupport();

private:
	VkInstance _vulkanInstance = nullptr;
	Extensions _extensions;
	Device _device;
};
