#pragma once

<<<<<<< HEAD
=======
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>
#include <vector>

>>>>>>> 932cff45debb381f728ecea216068e4fea6977b9
class Device
{
public:
	Device();
	virtual ~Device();

public:
<<<<<<< HEAD
	void Initialize();
};
=======
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

>>>>>>> 932cff45debb381f728ecea216068e4fea6977b9
