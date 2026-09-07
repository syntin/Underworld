// Author: RC

#include "commandBuffer.h"
#include "device.h"
#include "window.h"

CommandBuffer::CommandBuffer()
{
}

CommandBuffer::~CommandBuffer()
{
}

bool CommandBuffer::CreateCommandBuffers(Device device, Window window, uint32_t gfxQueueFamIdx, std::array<FrameResources, MaxFramesInFlight>& frameResources)
{
	for (FrameResources& res : _syncro.GetFrameResources())
	{
		// we'll give each frame its own pool, faster cmd buffer resets this way
		VkCommandPoolCreateInfo poolInfo
		{
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.queueFamilyIndex = gfxQueueFamIdx
		};
		if (vkCreateCommandPool(device.GetLogicalDevice(), &poolInfo, nullptr, &res._commandPool) != VK_SUCCESS)
		{
			showError("Unable to create command buffer pool", window.GetSDLWindow());
			return false;
		}

		// create the command buffer for this frame
		VkCommandBufferAllocateInfo cmdAllocInfo
		{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = res._commandPool,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1,
		};

		if (vkAllocateCommandBuffers(device.GetLogicalDevice(), &cmdAllocInfo, &res._commandBuffer) != VK_SUCCESS)
		{
			showError("Unable to allocate command buffer", window.GetSDLWindow());
			return false;
		}
	}
	return true;
}

