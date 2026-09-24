// Author: RC & DB

#include "instance.h"
//#include <GLFW/glfw3.h>
#include "utils.h"
#include <vector>
#include <Volk/volk.h>
#include <vma/vk_mem_alloc.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>



static VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
        vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != nullptr)
        return func(instance, pCreateInfo, pAllocator, pMessenger);

    return VK_ERROR_EXTENSION_NOT_PRESENT;
}


VulkanInstance::VulkanInstance()
{

}

VulkanInstance::~VulkanInstance()
{
	// God forbid nobody called this
	Destroy();
}

bool VulkanInstance::Create(SDL_Window* window)
{
    if (volkInitialize() != VK_SUCCESS)
    {
        showError("Error initializing Volk", nullptr);
        return false;
    }

    VkApplicationInfo appInfo{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Astoroth Engine",
        .apiVersion = VulkanVersion,
    };

    
    Uint32 instExtCount = 0;
    const char* const* sdlExtensions = SDL_Vulkan_GetInstanceExtensions(&instExtCount);
    if (!sdlExtensions)
    {
        showError("SDL_Vulkan_GetInstanceExtensions failed", nullptr);
        return false;
    }

    std::vector<const char*> extensions;
    extensions.reserve(instExtCount + 1);

    for (Uint32 i = 0; i < instExtCount; i++)
        extensions.push_back(sdlExtensions[i]);

    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    
    std::vector<const char*> requestedLayers{
        "VK_LAYER_KHRONOS_validation"
    };

    
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCreateInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugCreateInfo.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugCreateInfo.pfnUserCallback = debugCallback;
    debugCreateInfo.pUserData = nullptr;

    
    VkInstanceCreateInfo instCreateInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = &debugCreateInfo,   
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = static_cast<uint32_t>(requestedLayers.size()),
        .ppEnabledLayerNames = requestedLayers.data(),
        .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
        .ppEnabledExtensionNames = extensions.data()
    };

    if (vkCreateInstance(&instCreateInfo, nullptr, &_vulkanInstance) != VK_SUCCESS)
    {
        showError("Failed to create Vulkan instance", nullptr);
        return false;
    }

    volkLoadInstance(_vulkanInstance);

    
    VkDebugUtilsMessengerEXT debugMessenger;
    if (CreateDebugUtilsMessengerEXT(_vulkanInstance, &debugCreateInfo, nullptr, &debugMessenger) != VK_SUCCESS)
    {
        std::cerr << "Failed to create debug messenger!" << std::endl;
    }

    return true;
}


void VulkanInstance::Destroy()
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
