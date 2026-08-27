#include <vulkan/vulkan.h>
#include <SDL3/SDL_vulkan.h>
#include <iostream>
#include <vector>
#include "vulkanQueue.h"
#include "device.h"
#include "utils.h"

#define MAX_DEVICE_COUNT 64

VulkanQueue::VulkanQueue()
{

}

VulkanQueue::~VulkanQueue()
{

}

void VulkanQueue::Initialize(VkInstance instance, Device& device)
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

    const float qfpriorities{ 1.0f };
    _queueCI.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    _queueCI.queueFamilyIndex = queueFamily;
    _queueCI.queueCount = 1;
    _queueCI.pQueuePriorities = &qfpriorities;
    VkQueue queue{};
    vkGetDeviceQueue(device.GetDevice(), queueFamily, 0, &queue);
}