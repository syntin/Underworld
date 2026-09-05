#pragma once

#include "instance.h"

class VolkLoader
{
public:
	VolkLoader();
	virtual ~VolkLoader();

public:
	bool Initialize(VulkanInstance vulkanInstance, Extensions& extensions);
	VulkanInstance& GetVulkanInstance() { return _vulkanInstance; }

public:
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallbackVolk(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
};

VKAPI_ATTR VkBool32 VKAPI_CALL VolkLoader::debugCallbackVolk(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	{
		std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;
	}
	return VK_FALSE;
}




