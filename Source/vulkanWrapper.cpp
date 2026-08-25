#include "vulkanWrapper.h"

VulkanWrapper::VulkanWrapper()
{

}

VulkanWrapper::~VulkanWrapper()
{

}

void VulkanWrapper::InitializeVulkan(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	_vulkanInstance.CreateInstance();
	_debugMessenger.SetupDebugMessenger();
	_extensions.GetRequiredExtensions();
	window.CreateGLFWwindow(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}

void VulkanWrapper::Destroy()
{

}
