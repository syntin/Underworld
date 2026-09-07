#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>
#include "vma.h"
#include "logicalDevice.h"
#include "PhysicalDevice.h"
#include "instance.h"

Vma::Vma()
{
}

Vma::~Vma()
{
}

bool Vma::Initialize(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkInstance instance)
{
	VmaAllocatorCreateInfo allocatorInfo = {};
	allocatorInfo.physicalDevice = physicalDevice; // Set this to your physical device
	allocatorInfo.device = logicalDevice;         // Set this to your logical device
	allocatorInfo.instance = instance;       // Set this to your Vulkan instance
	VmaAllocator allocator;
	VkResult result = vmaCreateAllocator(&allocatorInfo, &allocator);
	if (result != VK_SUCCESS)
	{
		return false;
	}
	_allocator = allocator;
	return true;
}
