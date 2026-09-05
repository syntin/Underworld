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
	void Initialize();
	uint32_t Initialize(VkInstance instance, Device device);
	std::vector<VkPhysicalDevice>& GetDevices() { return _devices; }
	VkDevice GetPhysicalDevice() { return _physicalDevice; }

	uint32_t GetDeviceIndex() { return _deviceIndex; }

private:
	std::vector<VkPhysicalDevice> _devices;

	uint32_t _deviceIndex = 0;
	uint32_t _deviceCount = 0;
	VkDevice _physicalDevice;
};


