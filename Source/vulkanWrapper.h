#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include "window.h"
#include "debugMessenger.h"
#include "instance.h"
#include "validationLayer.h"
#include "extensions.h"
#include "vulkanQueue.h"


class VulkanWrapper
{
public:
	VulkanWrapper();
	virtual ~VulkanWrapper();

public:
	void InitializeVulkan(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
	void RunRenderLoop();
	VkInstance GetInstance() { return _vulkanInstance; }

protected:
	void Destroy();

private:
	VulkanInstance _vulkanInstance;
	VkInstance _vkInstance{};
	VulkanQueue _queue;
	DebugMessenger _debugMessenger{};
	Extensions _extensions{};
	Window _window{};
};
