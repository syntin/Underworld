#include <vector>
#include "device.h"
#include "extensions.h"
#include "utils.h"

Device::Device()
{

}

Device::~Device()
{

}

VkPhysicalDevice Device::FindPhysicalDevice(VkInstance& instance, VkSurfaceKHR& surface, SDL_Window* window)
{
	uint32_t physDeviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &physDeviceCount, nullptr);
	std::vector<VkPhysicalDevice> physicalDevices(physDeviceCount);
	vkEnumeratePhysicalDevices(instance, &physDeviceCount, physicalDevices.data());

	VkPhysicalDevice physicalDevice = nullptr;
	if (physDeviceCount)
	{
		// if you have issues, you can always just hardcode a GPU index while learning
		physicalDevice = physicalDevices[0]; // default to first GPU
		// look through list and see if a dGPU exists
		for (auto& pDev : physicalDevices)
		{
			VkPhysicalDeviceProperties props{};
			vkGetPhysicalDeviceProperties(pDev, &props);
			if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				physicalDevice = pDev;
				break;
			}
		}
	}

	// ensure the desired swapchain format is supported
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
	std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, surfaceFormats.data());

	bool formatSupported = false;
	for (const VkSurfaceFormatKHR& surfFormat : surfaceFormats)
	{
		if (surfFormat.format == _swapchainFormat)
		{
			formatSupported = true;
			break;
		}
	}
	if (!formatSupported)
	{
		showError("Requested swapchain format is not supported by the surface", window);
		return nullptr;
	}

	return physicalDevice;
}

bool Device::CreateDevice(VkPhysicalDevice* physicalDevice, VkQueue* gfxQueue, uint32_t gfxQueueFamIdx, SDL_Window* window)
{
	float queuePriority = 1.0f;
	std::vector<uint32_t> queueFamiles{ gfxQueueFamIdx };

	VkDeviceQueueCreateInfo gfxQueueInfo
	{
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.queueFamilyIndex = gfxQueueFamIdx,
		.queueCount = 1,
		.pQueuePriorities = &queuePriority
	};

	// query suppoted features
	VkPhysicalDeviceVulkan14Features supportedFeatures14{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES, .pNext = nullptr };
	VkPhysicalDeviceVulkan13Features supportedFeatures13{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES, .pNext = &supportedFeatures14 };
	VkPhysicalDeviceVulkan12Features supportedFeatures12{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES, .pNext = &supportedFeatures13 };
	VkPhysicalDeviceFeatures2 supportedFeatures{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, .pNext = &supportedFeatures12 };
	vkGetPhysicalDeviceFeatures2(*physicalDevice, &supportedFeatures);

	// check if what we need is supported
	if (!supportedFeatures13.dynamicRendering || !supportedFeatures13.synchronization2 ||
		!supportedFeatures12.timelineSemaphore)
	{
		showError("Physical device doesn't meet the feature requirements", window);
		return false;
	}

	// produce a separate features struct chain for device creation
	VkPhysicalDeviceVulkan14Features features14
	{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES,
		.pNext = nullptr,
	};
	VkPhysicalDeviceVulkan13Features features13
	{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
		.pNext = &features14,
		.synchronization2 = VK_TRUE,
		.dynamicRendering = VK_TRUE,
	};
	VkPhysicalDeviceVulkan12Features features12
	{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
		.pNext = &features13,
		.timelineSemaphore = VK_TRUE
	};
	VkPhysicalDeviceFeatures2 features{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, .pNext = &features12 };

	const std::vector<const char*> deviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	VkDeviceCreateInfo devCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext = &features,
		.queueCreateInfoCount = 1,
		.pQueueCreateInfos = &gfxQueueInfo,
		.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
		.ppEnabledExtensionNames = deviceExtensions.data(),
		.pEnabledFeatures = nullptr // features struct chain is set in pNext
	};

	if (vkCreateDevice(*physicalDevice, &devCreateInfo, nullptr, (VkDevice*) physicalDevice) != VK_SUCCESS)
	{
		return false;
	}

	// grab the VkQueue object finally
	vkGetDeviceQueue((*physicalDevice).GetDevice(), gfxQueueFamIdx, 0, &gfxQueue);
	if (!gfxQueue)
	{
		showError("Couldn't get the graphics queue", window);
		return false;
	}
	return true;
}
