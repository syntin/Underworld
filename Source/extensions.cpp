#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <iostream>
#include <stdio.h>
#include <cstdint>
#include <vector>
#include "extensions.h"

using namespace std;

Extensions::Extensions()
{

}

Extensions::~Extensions()
{

}

void Extensions::Initialize()
{
	SDL_Vulkan_GetInstanceExtensions(&_extensionCount);
	std::vector<const char*> requestedExtensions
	{
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
	};

	for (int i = 0; i < _extensionCount; ++i)
	{
		requestedExtensions.push_back(_extensions[i]);
	}

	std::vector<const char*> requestedLayers
	{
		"VK_LAYER_KHRONOS_validation"
	};

	VkDebugUtilsMessengerCreateInfoEXT debugInfo
	{
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
		.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
		.pfnUserCallback = debugCallback
	};

	vkEnumerateInstanceExtensionProperties(nullptr, &_extensionCount, nullptr);
	vector<VkExtensionProperties> extensions(_extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &_extensionCount, extensions.data());

	// Show this when using DEBUG mode
#ifndef NDEBUG
	std::cout << "available extensions:\n";
	for (const auto& extension : extensions) {
		std::cout << '\t' << extension.extensionName << '\n';
	}
#endif
	_extensions = SDL_Vulkan_GetInstanceExtensions(&_extensionCount);
}