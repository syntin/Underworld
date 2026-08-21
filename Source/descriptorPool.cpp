#include "descriptorPool.h"

void DescriptorPool::Create()
{
	VkDescriptorPoolSize poolSizesBindless[] = {
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, k_max_bindless_resources;  },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, k_max_bindless_resources; }
	};
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT;
	pool_info.maxSets = k_max_bindless_resources * ArraySize(pool_sizes_bindless);
	pool_info.poolSizeCount = (u32)ArraySize(pool_sizes_bindless);
	pool_info.pPoolSizes = pool_sizes_bindless;
	vkCreateDescriptorPool(vulkan_device, &pool_info, vulkan_allocation_callbacks, &vulkan_bindless_descriptor_pool);
}
