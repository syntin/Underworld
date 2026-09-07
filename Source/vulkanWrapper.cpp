// Author: RC

#include "vulkanWrapper.h"
#include <Windows.h>
#define VOLK_IMPLEMENTATION
#include <Volk/volk.h>
#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>
#include "utils.h"



VulkanWrapper::VulkanWrapper()
{

}

VulkanWrapper::~VulkanWrapper()
{
    Destroy();
}

void VulkanWrapper::InitializeVulkan(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	SDL_InitSubSystem(SDL_INIT_VIDEO);
	_window.SetSDLWindow(SDL_CreateWindow("Vulkan Learning", WIDTH, HEIGHT, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE));

	if (!_window.GetSDLWindow())
	{
		showError("Error creating window", _window.GetSDLWindow());
		return;
	}

	if (!InitializeVulkan())
	{
	}
}

bool VulkanWrapper::InitializeVulkan()
{
	if (!_instance.Create())
	{
		showError("Couldn't create a vulkan instance", _window.GetSDLWindow());
		return false;
	}

	if (!_surface.Create(_instance.GetInstance(), _window.GetSDLWindow()))
	{
		showError("Couldn't create window surface", _window.GetSDLWindow());
		return false;
	}

	if (!_device.FindPhysicalDevice(_instance.GetInstance(), &(_surface.GetSurface()), _window.GetSDLWindow()))
	{
		showError("Unable to find an appropriate physical device", _window.GetSDLWindow());
		return false;
	}

	if (!_graphicsQueue.FindGraphicsQueue(_surface.Data(), reinterpret_cast<Device*>(_device.GetPhysicalDevice())))
	{
		showError("Unable to find a compatible graphics queue", _window.GetSDLWindow());
		return false;
	}

	VkPhysicalDevice physicalDevice = _device.GetPhysicalDevice();
	if (!_device.Create(&physicalDevice, _graphicsQueue.GetGraphicsQueue(), _graphicsQueue.GetGraphicsQueueFamilyIndex(), _window.GetSDLWindow()))
	{
		showError("Couldn't create the logical GPU device", _window.GetSDLWindow());
		return false;
	}

	if (!_vma.Initialize(_device.GetPhysicalDevice(), _device.GetLogicalDevice(), _instance.GetInstance()))
	{
		showError("Unable to create Vulkan Memory Allocator", _window.GetSDLWindow());
		return false;
	}

	if (!_swapChain.Create(_device, _surface, _vma, _window.GetSDLWindow(), WIDTH, HEIGHT))
	{
		showError("Unable to create swapchain", _window.GetSDLWindow());
		return false;
	}


	if (!_shader.Create(_device))
	{
		showError("Error creating shader modules", _window.GetSDLWindow());
		return false;
	}

	if (_pipeline.Create(_device, _window, _swapChain, _shader); !_pipeline.GetPipeline())
	{
		showError("Unable to initialize the graphics pipeline", _window.GetSDLWindow());
		return false;
	}

	if (!_synchronization.Initialize(_device, _window))
	{
		showError("Couldn't create the sync related resources", _window.GetSDLWindow());
		return false;
	}

	if (!_commandBuffer.CreateCommandBuffers(_device, _window, _graphicsQueue.GetGraphicsQueueFamilyIndex(), _synchronization.GetFrameResources()))
	{
		showError("Couldn't create command buffer objects", _window.GetSDLWindow());
		return false;
	}

	return true;
}

void VulkanWrapper::Run()
{
	_running = true;
	while (_running)
	{
		SDL_Event event{ 0 };
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
			{
				_running = false;
				break;
			}
			else if (event.type == SDL_EVENT_WINDOW_RESIZED)
			{
				uint32_t width = event.window.data1;
				uint32_t height = event.window.data2;
				break;
			}
		}

		_bindlessRender.Render();
	}
}

void VulkanWrapper::Render()
{

}

void VulkanWrapper::Destroy()
{
	// wait in case resources are in use
	vkDeviceWaitIdle(_device.GetLogicalDevice());

	// frame / sync object cleanup
	if (_synchronization.GetTimelineSemaphore())
	{
		vkDestroySemaphore(_device.GetLogicalDevice(), _synchronization.GetTimelineSemaphore(), nullptr);
	}
	for (auto& res : _frameResources)
	{
		vkDestroySemaphore(_device.GetLogicalDevice(), res.imageAcquiredSemaphore, nullptr);
		vkDestroyCommandPool(_device.GetLogicalDevice(), res.commandPool, nullptr); // destroys buffers implicitly
	}

	// pipeline cleanup
	if (_pipeline.GetPipeline())
	{
		vkDestroyPipelineLayout(_device.GetLogicalDevice(), _pipeline.GetPipelineLayout(), nullptr);
		vkDestroyPipeline(_device.GetLogicalDevice(), _pipeline.GetPipeline(), nullptr);
	}

	// cleanup shaders
	if (_shader.GetVertShader())
	{
		vkDestroyShaderModule(_device.GetLogicalDevice(), _shader.GetVertShader(), nullptr);
	}
	if (_shader.GetFragShader())
	{
		vkDestroyShaderModule(_device.GetLogicalDevice(), _shader.GetFragShader(), nullptr);
	}

	// cleanup swapchain
	DestroySwapchain();

	// VMA
	if (_vma.GetAllocator())
	{
		vmaDestroyAllocator(_vma.GetAllocator());
	}

	// cleanup Vulkan
	if (_surface.GetSurface())
	{
		vkDestroySurfaceKHR(_instance.GetInstance(), _surface.GetSurface(), nullptr);
	}
	if (_device.GetLogicalDevice())
	{
		vkDestroyDevice(_device.GetLogicalDevice(), nullptr);
	}
	if (_instance.GetInstance())
	{
		vkDestroyInstance(_instance.GetInstance(), nullptr);
	}
	volkFinalize();

	// cleanup SDL
	if (_window.GetSDLWindow())
	{
		SDL_DestroyWindow(_window.GetSDLWindow());
	}
	SDL_Quit();
}

void VulkanWrapper::DestroySwapchain()
{
	/*
	for (VkImageView swapchainImgView : _swapChain.GetDepthImageView())
	{
		vkDestroyImageView(_device.GetLogicalDevice(), swapchainImgView, nullptr);
	}
	_swapChain.GetSwapChainImageViews().clear();

	// destroy render-complete ssemaphores
	for (VkSemaphore& semaphore : renderCompleteSemaphores)
	{
		vkDestroySemaphore(_device.GetLogicalDevice(), semaphore, nullptr);
	}
	renderCompleteSemaphores.clear();

	if (_swapChain.GetSwapChain())
	{
		vkDestroySwapchainKHR(_device.GetLogicalDevice(), _swapChain.GetSwapChain(), nullptr);
	}

	// destroy the depth buffer along with the swapchain
	if (depthImageView)
	{
		vkDestroyImageView(_device.GetLogicalDevice(), depthImageView, nullptr);
		vmaDestroyImage(_vma.GetAllocator(), depthImage, depthImageAllocation);
		depthImageView = nullptr;
	}
	*/
}


void VulkanWrapper::Run() {
	_running = true;
	while (_running) {
		SDL_Event event{0};
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				_running = false;
				break;
			}
			else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
				_newWidth = event.window.data1;
				_newHeight = event.window.data2;
				break;
			}
		}

		Render();
	}
}