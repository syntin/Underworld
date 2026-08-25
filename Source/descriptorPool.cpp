#include "descriptorPool.h"
#include <array>
#include <stdexcept>
#include "utils.h"

using namespace std;

DescriptorPool::DescriptorPool()
{
}

DescriptorPool::~DescriptorPool()
{
}

void DescriptorPool::Create(VkAllocationCallbacks* allocationCallback)
{
	VkDescriptorPoolSize poolSizesBindless[] = {
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, _kMaxBindlessResources },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, _kMaxBindlessResources }
	};
	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.pNext = nullptr;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT;
	poolInfo.maxSets = _kMaxBindlessResources * ArraySize(poolSizesBindless);
	poolInfo.poolSizeCount = ArraySize(poolSizesBindless);
	poolInfo.pPoolSizes = poolSizesBindless;

	VkDescriptorPool bindlessDescriptorPool = VK_NULL_HANDLE;
	VkResult result = vkCreateDescriptorPool(_vulkanDevice, &poolInfo, allocationCallback, &bindlessDescriptorPool);
	if (result != VK_SUCCESS) {
	    throw std::runtime_error("Failed to create Descriptor Pool");
	}
}
