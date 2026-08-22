#include "descriptorPool.h"
#include <array>

using namespace std;

void DescriptorPool::Create(VkAllocationCallbacks* allocationCallbacks)
{
	VkDescriptorPoolSize poolSizesBindless[] = {
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, _kMaxBindlessResources },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, _kMaxBindlessResources }
	};
	VkDescriptorPoolCreateInfo poolInfo;
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT;
	poolInfo.maxSets = _kMaxBindlessResources * ArraySize(poolSizesBindless);
	poolInfo.poolSizeCount = (uint32_t) ArraySize(poolSizesBindless);
	poolInfo.pPoolSizes = poolSizesBindless;
	vkCreateDescriptorPool(_vulkanDevice, &poolInfo, allocationCallbacks, &bindlessDescriptorPool);
	*/
}
