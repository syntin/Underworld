#pragma once

#include <cstdint>
#include <stdexcept>
#include <vector>
#include <iostream>

#pragma warning(disable: 4005)
#pragma warning(disable: 4098)

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

#ifndef ArraySize
	#define ArraySize(a) (sizeof(a) / sizeof((a)[0]))
#endif

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif


static inline void chk(VkResult result)
{
	if (result != VK_SUCCESS)
	{
		std::cerr << "Vulkan call returned an error (" << result << ")\n";
		exit(result);
	}
}
/*
static inline void chkSwapchain(VkResult result) {
	if (result < VK_SUCCESS) {
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			updateSwapchain = true;
			return;
		}
		std::cerr << "Vulkan call returned an error (" << result << ")\n";
		exit(result);
	}
}
*/
static inline void chk(bool result)
{
	if (!result)
	{
		std::cerr << "Call returned an error\n";
		exit(result);
	}
}


/*
static VKAPI_ATTR uint32_t VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	{
		fprintf(stderr, "Shitty Error: %s\n", pCallbackData->pMessage);
	}

	return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, 
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
	const VkAllocationCallbacks* pAllocator, 
	VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}
*/
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;


