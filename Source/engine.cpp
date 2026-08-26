#include "engine.h"
#include <iostream>
/*
const int WIDTH = 800;
const int HEIGHT = 600;

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
    std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}


void Engine_t::Run()
{
    InitWindow();
    InitVulkan();
    MainLoop();
    Cleanup();
}

void Engine_t::InitWindow()
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    _window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void Engine_t::InitVulkan()
{
	CreateInstance();
	SetupDebugMessenger();
	CreateSurface();
	PickPhysicalDevice();
	CreateLogicalDevice();
	CreateSwapchain();
	CreateImageViews();
	CreateRenderPass();
	CreateDescriptorSetLayout();
	CreateGraphicsPipeline();
	CreateCommandPool();
	CreateColorResources();
	CreateDepthResources();
	CreateFrameBuffers();
	CreateTextureImage();
	CreateTextureImageView();
	CreateTextureSampler();
	LoadModel();
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateUniformBuffers();
	CreateDescriptorPool();
	CreateDescriptorSets();
	CreateCommandBuffers();
	CreateSyncObjects();

	std::cout << "RGC Initialized!" << std::endl;
}

void Engine_t::MainLoop()
{
    while (!glfwWindowShouldClose(_window))
    {
        glfwPollEvents();
        DrawFrame();
    }

    vkDeviceWaitIdle(_device);
}

void Engine_t::DrawFrame()
{

}

void Engine_t::CreateInstance()
{

}

void Engine_t::SetupDebugMessenger()
{

}

void Engine_t::CreateSurface()
{

}

void Engine_t::PickPhysicalDevice()
{

}

void Engine_t::CreateLogicalDevice()
{

}

void Engine_t::CreateSwapchain()
{

}

void Engine_t::CreateImageViews()
{

}

void Engine_t::CreateRenderPass()
{

}

void Engine_t::CreateDescriptorSetLayout()
{

}

void Engine_t::CreateGraphicsPipeline()
{

}

void Engine_t::CreateCommandPool()
{

}

void Engine_t::CreateColorResources()
{

}

void Engine_t::CreateDepthResources()
{

}

void Engine_t::CreateFrameBuffers()
{

}

void Engine_t::CreateTextureImage()
{

}

void Engine_t::CreateTextureImageView()
{

}

void Engine_t::CreateTextureSampler()
{

}

void Engine_t::LoadModel()
{

}

void Engine_t::CreateVertexBuffer()
{

}

void Engine_t::CreateIndexBuffer()
{

}

void Engine_t::CreateUniformBuffers()
{

}

void Engine_t::CreateDescriptorPool()
{

}

void Engine_t::CreateDescriptorSets()
{

}

void Engine_t::CreateCommandBuffers()
{

}

void Engine_t::CreateSyncObjects()
{

}

void Engine_t::Cleanup()
{

}
*/