#include <vulkan/vulkan.h>
#include <SDL3/SDL_vulkan.h>
#include <iostream>
#include <vector>
#include "vulkanQueue.h"

#define MAX_DEVICE_COUNT 64

VulkanQueue::VulkanQueue()
{

}

VulkanQueue::~VulkanQueue()
{

}

void VulkanQueue::Initialize(VkInstance instance)
{
    VkPhysicalDeviceProperties2 deviceProperties{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2 };
    VkPhysicalDevice physicalDevice[MAX_DEVICE_COUNT] = {};
    uint32_t deviceIndex = 0;
    vkGetPhysicalDeviceProperties2(physicalDevice[deviceIndex], &deviceProperties);
    std::cout << "Selected device: " << deviceProperties.properties.deviceName << "\n";

    uint32_t queueFamilyCount{};
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice[deviceIndex], &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice[deviceIndex], &queueFamilyCount, queueFamilies.data());
    uint32_t queueFamily{};
    for (size_t i = 0; i < queueFamilies.size(); i++)
    {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            queueFamily = static_cast<uint32_t>(i);
            break;
        }
    }
    if (SDL_Vulkan_GetPresentationSupport(instance, physicalDevice[deviceIndex], queueFamily) == VK_FALSE)
    {
        throw std::runtime_error("Failed to create presentation layer");
    }
    const float qfpriorities{ 1.0f };
    VkDeviceQueueCreateInfo queueCI{
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = queueFamily,
        .queueCount = 1,
        .pQueuePriorities = &qfpriorities
    };
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
    VkPhysicalDeviceFeatures enabledVk10Features{
        .samplerAnisotropy = VK_TRUE
    };
    VkDeviceCreateInfo deviceCI{
    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .pNext = &enabledVk13Features,
    .queueCreateInfoCount = 1,
    .pQueueCreateInfos = &queueCI,
    .enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
    .ppEnabledExtensionNames = deviceExtensions.data(),
    .pEnabledFeatures = &enabledVk10Features
    };
    VkDevice device{};
    VkQueue queue{};
    vkGetDeviceQueue(device, queueFamily, 0, &queue);
}