#include "commandBuffer.h"

CommandBuffer::CommandBuffer()
{
}

CommandBuffer::~CommandBuffer()
{
}

bool CommandBuffer::CreateCommandBuffers(VkDevice device, uint32_t gfxQueueFamIdx, std::array<FrameResources, MaxFramesInFlight>& frameResources)
{
	for (FrameResources& res : _syncro.GetFrameResources())
	{
		// we'll give each frame its own pool, faster cmd buffer resets this way
		VkCommandPoolCreateInfo poolInfo
		{
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.queueFamilyIndex = gfxQueueFamIdx
		};
		if (vkCreateCommandPool(device, &poolInfo, nullptr, &res._commandPool) != VK_SUCCESS)
		{
			showError("Unable to create command buffer pool");
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

		if (vkAllocateCommandBuffers(device, &cmdAllocInfo, &res._commandBuffer) != VK_SUCCESS)
		{
			showError("Unable to allocate command buffer");
			return false;
		}
	}
	return true;
}

