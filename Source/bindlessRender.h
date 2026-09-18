// Author: RC & DB

#pragma once

#include <volk/volk.h>
#include "device.h"
#include "swapchain.h"
#include "graphicsQueue.h"
#include "synchronization.h"
#include "commandBuffer.h"
#include "graphicsPipeline.h"

class BindlessRender
{
public:
	BindlessRender();
	virtual ~BindlessRender();

	// Called once from VulkanWrapper::SetupVulkan()
	void Initialize(
		Device* device,
		SwapChain* swapChain,
		GraphicsQueue* graphicsQueue,
		Synchronization* sync,
		CommandBuffer* cmdBuffer,
		GraphicsPipeline* pipeline);


public:
	VkPhysicalDeviceDescriptorIndexingFeatures QueryDeviceForBindlessSupport();
	void CreateAndEnableBindlessDevice(VkPhysicalDeviceDescriptorIndexingFeatures indexingFeatures, VkDevice vulkanDevice);
	void Render();

private:
	bool _bindlessSupported = false;
	bool _bindlessEnabled = false;
	VkPhysicalDevice _vulkanPhysicalDevice;
	
	// Vulkan objects needed to record a frame
	Device* _device = nullptr;
	SwapChain* _swapChain = nullptr;
	GraphicsQueue* _graphicsQueue = nullptr;
	Synchronization* _sync = nullptr;
	CommandBuffer* _cmdBuffer = nullptr;
	GraphicsPipeline* _pipeline = nullptr;

	// Frame counters
	uint32_t _frameIndex = 0;
	uint64_t _nextSignalValue = 0;
};