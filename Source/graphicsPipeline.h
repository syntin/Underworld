// Author: RC

#pragma once

#include <vulkan/vulkan.h>
#include "device.h"
#include "window.h"
#include "swapchain.h"
#include "shader.h"
#include "utils.h"


class GraphicsPipeline
{
public:
	GraphicsPipeline();
	virtual ~GraphicsPipeline();

public:
	VkPipeline Create(Device& device, Window window, SwapChain swapChain, Shader shader);
	VkPipeline GetPipeline() { return _pipeline; }
	VkPipelineLayout GetPipelineLayout(){ return _pipelineLayout; }

private:
	VkPipelineLayout _pipelineLayout{ VK_NULL_HANDLE };
	VkPipeline _pipeline = nullptr;
	Device _device{};

};