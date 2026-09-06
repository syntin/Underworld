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
		showError("Couldn't create a vulkan instance");
		return false;
	}

	if (!_surface.Create())
	{
		showError("Couldn't create window surface");
		return false;
	}

	if (!_physicalDevice.FindPhysicalDevice())
	{
		showError("Unable to find an appropriate physical device");
		return false;
	}

	if (!_graphicsQueue.FindGraphicsQueue())
	{
		showError("Unable to find a compatible graphics queue");
		return false;
	}

	if (!_physicalDevice.Create())
	{
		showError("Couldn't create the logical GPU device");
		return false;
	}

	if (!_vma.Initialize())
	{
		showError("Unable to create Vulkan Memory Allocator");
		return false;
	}

	if (!_swapchain.Create(width, height))
	{
		showError("Unable to create swapchain");
		return false;
	}

	if (!_shaders.Create())
	{
		showError("Error creating shader modules");
		return false;
	}

	if (_pipeline.Create(); !_pipeline)
	{
		showError("Unable to initialize the graphics pipeline");
		return false;
	}




	if (!createSyncResources())
	{
		showError("Couldn't create the sync related resources");
		return false;
	}

	if (!createCommandBuffers())
	{
		showError("Couldn't create command buffer objects");
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
				running = false;
				break;
			}
			else if (event.type == SDL_EVENT_WINDOW_RESIZED)
			{
				uint32_t width = event.window.data1;
				uint32_t height = event.window.data2;
				break;
			}
		}

		Render();
	}
}

void VulkanWrapper::Render()
{

}

void VulkanWrapper::Destroy()
{
	// wait in case resources are in use
	vkDeviceWaitIdle(_device);

	// frame / sync object cleanup
	if (_timelineSemaphore)
	{
		vkDestroySemaphore(_device, _timelineSemaphore, nullptr);
	}
	for (auto& res : _frameResources)
	{
		vkDestroySemaphore(_vulkanDevice, res.imageAcquiredSemaphore, nullptr);
		vkDestroyCommandPool(_vulkanDevice, res.commandPool, nullptr); // destroys buffers implicitly
	}

	// pipeline cleanup
	if (_pipelineLayout)
	{
		vkDestroyPipelineLayout(device, _pipelineLayout, nullptr);
	}
	if (_pipeline)
	{
		vkDestroyPipeline(device, _pipeline, nullptr);
	}

	// cleanup shaders
	if (_vertShader)
	{
		vkDestroyShaderModule(device, _vertShader, nullptr);
	}
	if (_fragShader)
	{
		vkDestroyShaderModule(device, _fragShader, nullptr);
	}

	// cleanup swapchain
	destroySwapchain();

	// VMA
	if (_vmaAllocator)
	{
		vmaDestroyAllocator(_vmaAllocator);
	}

	// cleanup Vulkan
	if (_surface)
	{
		vkDestroySurfaceKHR(_vulkanInstance, _surface, nullptr);
	}
	if (_device)
	{
		vkDestroyDevice(_device, nullptr);
	}
	if (_vulkanInstance)
	{
		vkDestroyInstance(_vulkanInstance, nullptr);
	}
	volkFinalize();

	// cleanup SDL
	if (_SDLwindow)
	{
		SDL_DestroyWindow(_SDLwindow);
	}
	SDL_Quit();
}

void VulkanWrapper::Run() {
	_running = true;
	while (_running) {
		SDL_Event event{0};
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
				break;
			}
			else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
				_width = event.window.data1;
				_height = event.window.data2;
				break;
			}
		}

		Render();
	}
}