#pragma once

#include "window.h"
#include "debugMessenger.h"
#include "instance.h"
#include "validationLayer.h"
#include "extensions.h"


class VulkanWrapper
{
public:
	VulkanWrapper();
	virtual ~VulkanWrapper();

public:
	InitializeVulkan();

private:
	VulkanInstance _vulkanInstance;
	DebugMessenger _debugMessenger;
	Extensions _extensions;
	Window _window;
};