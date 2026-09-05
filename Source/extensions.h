#pragma once
#include <vector>
#include "utils.h"

class Extensions
{
public:
	Extensions();
	virtual ~Extensions();

public:
	void Initialize();
	uint32_t GetExtensionsCount() { return _extensionCount; }
	char const* const* Data() { return _extensions;  }

private:
	std::vector<const char*> _extensionVector;
	uint32_t _extensionCount = 0;
	char const* const* _extensions = nullptr;

public:
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

};

VKAPI_ATTR VkBool32 VKAPI_CALL Extensions::debugCallback(
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


