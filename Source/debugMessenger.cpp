#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include "debugMessenger.h"
#include "utils.h"
#include <cstdint>

DebugMessenger::DebugMessenger()
{

}

DebugMessenger::~DebugMessenger()
{

}

void DebugMessenger::Initialize()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	_extensions = std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
		_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

}