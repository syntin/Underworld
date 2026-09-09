// Author: RC

#include <Windows.h>
#include "graphicsPipeline.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

GraphicsPipeline::GraphicsPipeline()
{
}

GraphicsPipeline::~GraphicsPipeline()
{
	if (_pipeline)
	{
		vkDestroyPipelineLayout(_device.GetLogicalDevice(), _pipelineLayout, nullptr);
		vkDestroyPipeline(_device.GetLogicalDevice(), _pipeline, nullptr);
	}
}
VkPipeline GraphicsPipeline::Create(Device& device, Window window, SwapChain swapChain, Shader shader)
{
	_device = device;
	// need to define a pipeline layout
	VkPipelineLayoutCreateInfo pipelineLayoutInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 0,
		.pushConstantRangeCount = 0
	};
	if (vkCreatePipelineLayout(device.GetLogicalDevice(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
	{
		showError("Unable to create the pipeline layout", window.GetSDLWindow());
		return nullptr;
	}

	// configure the shader stages struct
	const char* entryPoint = "main";
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages
	{
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = VK_SHADER_STAGE_VERTEX_BIT,
			.module = shader.GetVertShader(),
			.pName = entryPoint
		},
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
			.module = shader.GetFragShader(),
			.pName = entryPoint
		}
	};

	// vertex pulling, don't define vertex input details
	VkPipelineVertexInputStateCreateInfo vertInputInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO
	};

	// input assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
	};

	// depth/stencil configuration
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		.depthTestEnable = VK_TRUE,
		.depthWriteEnable = VK_TRUE,
		.depthCompareOp = VK_COMPARE_OP_LESS,
		.stencilTestEnable = VK_FALSE
	};

	// dynamic rendering allows to set this up
	VkPipelineViewportStateCreateInfo viewportInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = 1,
		.pViewports = nullptr,
		.scissorCount = 1,
		.pScissors = nullptr
	};

	// rasterizer settings
	VkPipelineRasterizationStateCreateInfo rasterInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.polygonMode = VK_POLYGON_MODE_FILL,
		.cullMode = VK_CULL_MODE_BACK_BIT,
		.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
		.lineWidth = 1.0f,
	};

	// No multisampling
	VkPipelineMultisampleStateCreateInfo multiSampleInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT
	};

	// Alpha-blending (disabled for now), still need attachment info and write mask
	VkPipelineColorBlendAttachmentState attachState
	{
		.blendEnable = VK_FALSE,
		.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
	};
	VkPipelineColorBlendStateCreateInfo blendInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.attachmentCount = 1,
		.pAttachments = &attachState
	};

	// enable dynamic state
	std::vector<VkDynamicState> dynamicState
	{
		VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR
	};
	VkPipelineDynamicStateCreateInfo dynamicStateInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.dynamicStateCount = static_cast<uint32_t>(dynamicState.size()),
		.pDynamicStates = dynamicState.data()
	};

	// structure required for dynamic rendering
	VkPipelineRenderingCreateInfo renderInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
		.colorAttachmentCount = 1,
		.pColorAttachmentFormats = swapChain.GetSwapChainFormat(),
		.depthAttachmentFormat = depthFormat
	};

	// Create the graphics pipeline
	VkGraphicsPipelineCreateInfo pipelineInfo
	{
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.pNext = &renderInfo,
		.stageCount = static_cast<uint32_t>(shaderStages.size()),
		.pStages = shaderStages.data(),
		.pVertexInputState = &vertInputInfo,
		.pInputAssemblyState = &inputAssemblyInfo,
		.pViewportState = &viewportInfo,
		.pRasterizationState = &rasterInfo,
		.pMultisampleState = &multiSampleInfo,
		.pDepthStencilState = &depthStencilInfo,
		.pColorBlendState = &blendInfo,
		.pDynamicState = &dynamicStateInfo,
		.layout = _pipelineLayout,
		.renderPass = VK_NULL_HANDLE,
	};

	if (vkCreateGraphicsPipelines(device.GetLogicalDevice(), nullptr, 1, &pipelineInfo, nullptr, &_pipeline) != VK_SUCCESS)
	{
		showError("Error creating the pipeline", window.GetSDLWindow());
		return nullptr;
	}
	return _pipeline;
}