// Author: RC

#include <SDL3/SDL_vulkan.h>
#include <volk/volk.h>
#include <iostream>
#include <vector>
#include "graphicsQueue.h"
#include "device.h"
#include "surface.h"
#include "utils.h"

#define MAX_DEVICE_COUNT 64

GraphicsQueue::GraphicsQueue()
{

}

GraphicsQueue::~GraphicsQueue()
{

}

void GraphicsQueue::Create(VkInstance& instance, Device device)
{
    uint32_t queueFamilyCount{0};
    std::vector<VkPhysicalDevice> devices = device.GetDevices();
    vkGetPhysicalDeviceQueueFamilyProperties(devices[device.GetDeviceIndex()], &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(devices[device.GetDeviceIndex()], &queueFamilyCount, queueFamilies.data());
    uint32_t queueFamily{0};
    for (size_t i = 0; i < queueFamilies.size(); i++)
    {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            queueFamily = static_cast<uint32_t>(i);
            break;
        }
    }
    chk(SDL_Vulkan_GetPresentationSupport(instance, devices[device.GetDeviceIndex()], queueFamily));
}

bool GraphicsQueue::FindGraphicsQueue(Surface* surface, Device* device)
{
	// eventually we'll have more complex queue lookup for presentation, etc
	// grab all of the queue families
	uint32_t queueFamCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties2(device->GetPhysicalDevice(), &queueFamCount, nullptr);
	std::vector<VkQueueFamilyProperties2> queueFamProps(queueFamCount, { VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2 });
	vkGetPhysicalDeviceQueueFamilyProperties2(device->GetPhysicalDevice(), &queueFamCount, queueFamProps.data());

	for (int currentFamIdx = 0; currentFamIdx < queueFamProps.size(); currentFamIdx++)
	{
		// ensure it has presentation support
		VkBool32 hasPresentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device->GetPhysicalDevice(), currentFamIdx, surface->GetSurface(), &hasPresentSupport);

		const auto& props = queueFamProps[currentFamIdx];
		// ensure this is a GRAPHICS queue with presentation support
		if (props.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT && hasPresentSupport)
		{
			_gfxQueueFamIdx = currentFamIdx;
			return true;
		}
	}
	return false;
}