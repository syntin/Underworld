#include "vulkanWrapper.h"

VulkanWrapper::InitializeVulkan()
{
	_vulkanInstance.CreateInstance();
	_debugMessenger.SetupDebugMessenger();
	if(CheckValidationSupport())
		_extensions.GetRequiredExtensions();
	window.CreateGLFWwindow();
}
