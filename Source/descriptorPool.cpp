#include "descriptorPool.h"

DescriptorPool::DescriptorPool()
{

}

DescriptorPool::~DescriptorPool()
{

}

void DescriptorPool::Create(VkDevice vulkanDevice)
{
	uint32_t kMaxBindlessResources;
	VkDescriptorPoolSize poolSizesBindless[] =
	{
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, kMaxBindlessResources  },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, kMaxBindlessResources }
	};

	VkDescriptorPoolCreateInfo poolInfo{
		.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT,
		.maxSets = kMaxBindlessResources * ArraySize(poolSizesBindless),
		.poolSizeCount = (uint32_t)ArraySize(poolSizesBindless),
		.pPoolSizes = poolSizesBindless
	};
	vkCreateDescriptorPool(vulkanDevice, &poolInfo, nullptr, &vulkanBindlessDescriptorPool);
}
