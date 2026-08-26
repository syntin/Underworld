#include "vulkanWrapper.h"

VulkanWrapper::VulkanWrapper()
{

}

VulkanWrapper::~VulkanWrapper()
{

}

void VulkanWrapper::InitializeVulkan(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nShowCmd)
{
	_vulkanInstance.CreateInstance();
	_debugMessenger.SetupDebugMessenger();
	_extensions.GetRequiredExtensions();
	_window.CreateGLFWwindow(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}

void VulkanWrapper::Destroy()
{

}