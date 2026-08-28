#include <vulkan/vulkan.h>
#include "vulkanWrapper.h"
#include "utils.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>


VulkanWrapper::VulkanWrapper()
{

}

VulkanWrapper::~VulkanWrapper()
{
    Destroy();
}

void VulkanWrapper::InitializeVulkan(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    chk(SDL_Init(SDL_INIT_VIDEO));
    chk(SDL_Vulkan_LoadLibrary(NULL));

    _vulkanInstance.Initialize();
    _device.Initialize(_vulkanInstance.GetInstance(), _device);
    _queue.Initialize(_vulkanInstance.GetInstance(), _device);
	_debugMessenger.Initialize();
	_extensions.Initialize();
	_window.CreateSDLwindow(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
    _swapChain.Initialize();
    _depthAttachment.Initialize();
    _mesh.Initialize();
    _vertexData.Initialize();
    _shaderData.Initialize();
    _synchronization.Initialize();
    _commandPool.Initialize();

    _textureImages.Initialize();
    _descriptor.Initialize();
    _slangShader.Initialize();
    _loadShader.Initialize();
    _pipeline.Initialize();

    RunRenderLoop();
    Destroy();
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