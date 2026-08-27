#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>
#include <vector>

class Device
{
public:
	Device();
	virtual ~Device();

public:
	uint32_t Initialize(VkInstance instance, Device device);
	std::vector<VkPhysicalDevice>& GetDevices() { return _devices; }
	VkDevice GetDevice() { return _device; }
	VkDeviceCreateInfo GetDeviceCreateInfo() { return _deviceCreateInfo; }
	VkDeviceQueueCreateInfo GetQueueCreateInfo() { return _deviceQueueCreateInfo; }

	uint32_t GetDeviceIndex() { return _deviceIndex; }

private:
	std::vector<VkPhysicalDevice> _devices;

	uint32_t _deviceIndex = 0;
	VkDevice _device;
	VkDeviceCreateInfo _deviceCreateInfo{};
	VkDeviceQueueCreateInfo _deviceQueueCreateInfo{};
};

