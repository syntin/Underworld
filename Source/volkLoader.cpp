#include <stdexcept>
#include <stdio.h>
#include <Volk/volk.h>
#include <SDL3/SDL_vulkan.h>
#include "volkLoader.h"
#include "utils.h"


VolkLoader::VolkLoader()
{
}

VolkLoader::~VolkLoader()
{

}

bool VolkLoader::Initialize()
{
	if (volkInitialize() != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to initialize volk");
	}

	VkApplicationInfo appInfo
	{
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Astoroth Engine",
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName = "Astoroth Engine",
		.engineVersion = VK_MAKE_VERSION(1, 0, 0),
		.apiVersion = VulkanVersion
	};

	uint32_t instExtCount = 0;
	const char* const* extensions = SDL_Vulkan_GetInstanceExtensions(&instExtCount);
	std::vector<const char*> requestedExtensions
	{
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
	};

	for (int i = 0; i < instExtCount; ++i)
	{
		requestedExtensions.push_back(extensions[i]);
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
		.pfnUserCallback = VolkLoader::debugCallback
	};

	VkInstanceCreateInfo instCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = &debugInfo,
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = static_cast<uint32_t>(requestedLayers.size()),
		.ppEnabledLayerName = requestedLayers.data(),
		.enabledExtensionCount = static_cast<uint32_t>(requestedExtensions.size()),
		.ppEnabledExtensionNames = requestedExtensions.data()
	};

	if (vkCreateInstance(&instCreateInfo, nullptr, &_vulkanInstance) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create Vulkan Instance");
		return false;
	}

	volkLoadInstance(_vulkanInstance);
	return true;
}


