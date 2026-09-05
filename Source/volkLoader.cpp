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
	volkFinalize();
}

bool VolkLoader::Initialize(VulkanInstance vulkanInstance, Extensions& extensions)
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

	
	// _vulkanInstance.Data() returns a pointer to the VkInstance handle, which is what vkCreateInstance
	// expects for its output parameter. Seems out of place but initialization needs to be called AFTER
	// creation of the extensions
	extensions.Initialize();
	vulkanInstance.Initialize(&appInfo);
	volkLoadInstance(vulkanInstance.GetInstance());
	return true;
}


