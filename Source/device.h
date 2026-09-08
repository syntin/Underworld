// Author: RC

#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <cstdint>
#include <iostream>
#include <vector>

class Device
{
public:
	Device();
	virtual ~Device();

public:
	VkPhysicalDevice FindPhysicalDevice(VkInstance& instance, VkSurfaceKHR& surface, SDL_Window* window);
	bool Create(VkPhysicalDevice* physicalDevice, VkQueue* gfxQueue, uint32_t gfxQueueFamIdx, SDL_Window* window);
	std::vector<VkPhysicalDevice>& GetDevices() { return _devices; }

public:
	uint32_t GetDeviceIndex() { return _deviceIndex; }
	uint32_t GetDeviceCount() { return _deviceCount; }
	VkPhysicalDevice GetPhysicalDevice() { return _physicalDevice; }
	VkDevice GetLogicalDevice() { return _logicalDevice; }

private:
	std::vector<VkPhysicalDevice> _devices;
	uint32_t _deviceIndex = 0;
	uint32_t _deviceCount = 0;
	VkPhysicalDevice _physicalDevice = nullptr;
	VkDevice _logicalDevice = nullptr;
	VkFormat _swapchainFormat = VK_FORMAT_B8G8R8A8_SRGB;
};


