#include "instance.h"
#include <GLFW/glfw3.h>
#include "utils.h"
#include <vector>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

VulkanInstance::VulkanInstance() :
	_instance(VK_NULL_HANDLE)
{

}

VulkanInstance::~VulkanInstance()
{

}

void VulkanInstance::Initialize()
{
}

void VulkanInstance::DestroyInstance()
{
	if (_instance)
	{
		vkDestroyInstance(_instance, nullptr);
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
