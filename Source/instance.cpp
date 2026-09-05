#include "instance.h"
#include <GLFW/glfw3.h>
#include "utils.h"
#include <vector>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

VulkanInstance::VulkanInstance()
{

}

VulkanInstance::~VulkanInstance()
{
	// God forbid nobody called this
	DestroyInstance();
}

bool VulkanInstance::Initialize(VkApplicationInfo* appInfo)
{
	// Not much to do here since it is initialized in VOLKLoader,
	// but we can check for validation layer support here
	if(CheckValidationLayerSupport() == false)
	{
		throw std::runtime_error("Validation layers requested, but not available!");
	}

	VkInstanceCreateInfo instCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = &debugCallbackInstance,
		.pApplicationInfo = appInfo,
		.enabledLayerCount = static_cast<uint32_t>(requestedLayers.size()),
		.ppEnabledLayerName = requestedLayers.data(),
		.enabledExtensionCount = static_cast<uint32_t>(requestedExtensions.size()),
		.ppEnabledExtensionNames = requestedExtensions.data()
	};

	if(vkCreateInstance(&instCreateInfo, nullptr, &_vulkanInstance) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create Vulkan instance!");
		return false;
	}
}

void VulkanInstance::DestroyInstance()
{
	if (_vulkanInstance != nullptr)
	{
		vkDestroyInstance(_vulkanInstance, nullptr);
		_vulkanInstance = nullptr;
	}
}

bool VulkanInstance::CheckValidationLayerSupport()
{
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}
