#include "vulkanWrapper.h"
#include <Windows.h>
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
    chk(SDL_InitSubSystem(SDL_INIT_VIDEO));
	_window.SetSDLWindow(SDL_CreateWindow("Underworld", WIDTH, HEIGHT, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE));
	SDL_Vulkan_LoadLibrary(NULL);

	// This will call a couple of Initialization functions in the proper order
    _volkLoader.Initialize(_vulkanInstance);

	VkApplicationInfo appInfo
	{
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Astoroth Engine",
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName = "Astoroth Engine",
		.engineVersion = VK_MAKE_VERSION(1, 0, 0),
		.apiVersion = VulkanVersion
	};





	_device.Initialize(_vulkanInstance.GetInstance(), _device);
    _queue.Initialize(_vulkanInstance.GetInstance(), _device);

	_debugMessenger.Initialize();
	_extensions.Initialize();
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

	// When RenderLoop exits, we need to clean up resources
    Destroy();
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