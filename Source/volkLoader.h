#pragma once

class VolkLoader
{
public:
	VolkLoader();
	virtual ~VolkLoader();

public:
	bool Initialize();
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

private:
	VkInstance _vulkanInstance = nullptr;
};

VKAPI_ATTR VkBool32 VKAPI_CALL VolkLoader::debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SECERITY_WARNING_BIT_EXT)
	{
		std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;
	}
	return VK_FALSE;
}




