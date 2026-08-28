#include "vulkanWrapper.h"


VulkanWrapper::VulkanWrapper()
{

}

VulkanWrapper::~VulkanWrapper()
{
    Destroy();
}

void VulkanWrapper::InitializeVulkan(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    _vulkanInstance.CreateInstance();
    _queue.Initialize(_vkInstance);
	_debugMessenger.SetupDebugMessenger();
	_extensions.GetRequiredExtensions();

	// The next line will create the basic window then create the GLFW window
	_window.CreateGLFWwindow(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
    RunRenderLoop();
}

void VulkanWrapper::RunRenderLoop()
{
    HWND hWnd = _window.GetHWnd();
    // Run the message loop
    MSG msg = { };
    while (PeekMessage(&msg, hWnd, 0, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void VulkanWrapper::Destroy()
{
    HWND hWnd = _window.GetHWnd();
    DestroyWindow(hWnd);
}