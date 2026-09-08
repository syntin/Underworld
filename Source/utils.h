// Author: RC

#pragma once

#include <cstdint>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <SDL3/SDL.h>

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

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

static inline void chk(bool result)
{
	if (!result)
	{
		std::cerr << "Call returned an error\n";
		exit(result);
	}
}
static inline void showError(const std::string& errorMessasge, SDL_Window* window)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", errorMessasge.c_str(), window);
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
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

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

constexpr static uint32_t VulkanVersion = VK_API_VERSION_1_4;
constexpr static uint32_t MaxFramesInFlight{ 2 };
constexpr static VkFormat swapChainFormat{ VK_FORMAT_B8G8R8A8_SRGB };
constexpr static VkFormat depthFormat{ VK_FORMAT_D32_SFLOAT };


