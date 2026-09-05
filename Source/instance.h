#pragma once
#include <vulkan/vulkan.h>
#include "extensions.h"
#include "device.h"

class VulkanInstance
{
public:
	VulkanInstance();
	virtual ~VulkanInstance();

public:
	bool Initialize(VkApplicationInfo* appInfo);
	void DestroyInstance();

public:
	VkInstance& GetInstance() { return _vulkanInstance; }
	VkInstance* Data() { return &_vulkanInstance; }
	void SetInstance(VkInstance instance) { _vulkanInstance = instance; }

protected:
	bool CheckValidationLayerSupport();

private:
	VkInstance _vulkanInstance = nullptr;
	Extensions _extensions;
	Device _device;

public:
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallbackInstance(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
};

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::debugCallbackInstance(
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
