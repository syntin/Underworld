#pragma once

#include <vulkan/vulkan.h>
//#define GLFW_DLL
//#include <GLFW/glfw3.h>
#include <GLFW/glfw3.h>


class Engine_t
{
public:
	Engine_t();
	virtual ~Engine_t();

public:
	void Run();

protected:
private:
	void InitWindow();
	void InitVulkan();
	void MainLoop();
	void DrawFrame();
	void Cleanup();
	void UpdateUniformBuffer(uint32_t currentImage);
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

private:
	void CreateInstance();
	void SetupDebugMessenger();
	void CreateSurface();
	void PickPhysicalDevice();
	void CreateLogicalDevice();
	void CreateSwapchain();
	void CreateImageViews();
	void CreateRenderPass();
	void CreateDescriptorSetLayout();
	void CreateGraphicsPipeline();
	void CreateCommandPool();
	void CreateColorResources();
	void CreateDepthResources();
	void CreateFrameBuffers();
	void CreateTextureImage();
	void CreateTextureImageView();
	void CreateTextureSampler();
	void LoadModel();
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreateUniformBuffers();
	void CreateDescriptorPool();
	void CreateDescriptorSets();
	void CreateCommandBuffers();
	void CreateSyncObjects();

private:
	GLFWwindow* _window;
	VkDevice _device;
};