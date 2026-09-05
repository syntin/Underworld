#include "device.h"
#include <vector>
#include "utils.h"

Device::Device()
{

}

Device::~Device()
{

}

uint32_t Device::Initialize(VkInstance instance, Device device)
{
    /*
	uint32_t deviceCount{ 0 };
	chk(vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr));
	device.resize((const size_t) deviceCount);
	chk(vkEnumeratePhysicalDevices(instance, &deviceCount, _devices.data()));

    VkPhysicalDeviceFeatures enabledVk10Features{ .samplerAnisotropy = VK_TRUE };
    const std::vector<const char*> deviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    VkPhysicalDeviceVulkan12Features enabledVk12Features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        .descriptorIndexing = true,
        .shaderSampledImageArrayNonUniformIndexing = true,
        .descriptorBindingVariableDescriptorCount = true,
        .runtimeDescriptorArray = true,
        .bufferDeviceAddress = true
    };
    VkPhysicalDeviceVulkan13Features enabledVk13Features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .pNext = &enabledVk12Features,
        .synchronization2 = true,
        .dynamicRendering = true,
    };
    
    VkDeviceQueueCreateInfo qInfo = device.GetQueueCreateInfo();
    _deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    _deviceCreateInfo.pNext = &enabledVk13Features,
    _deviceCreateInfo.queueCreateInfoCount = 1,
    _deviceCreateInfo.pQueueCreateInfos = &qInfo,
    _deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
    _deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data(),
    _deviceCreateInfo.pEnabledFeatures = &enabledVk10Features;
    chk(vkCreateDevice(_devices[_deviceIndex], &_deviceCreateInfo, nullptr, &_device));

	VkPhysicalDeviceProperties2 deviceProperties{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2 };
	vkGetPhysicalDeviceProperties2(_devices[_deviceIndex], &deviceProperties);
	std::cout << "Selected device: " << deviceProperties.properties.deviceName << "\n";
    */
	return _deviceCount;
}
