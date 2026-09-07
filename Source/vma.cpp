// Author: RC

#include <vulkan/vulkan.h>
#include <Volk/volk.h>
#include <vma/vk_mem_alloc.h>
#include "vma.h"
#include "logicalDevice.h"
#include "Device.h"
#include "instance.h"

Vma::Vma()
{
}

Vma::~Vma()
{
}

bool Vma::Initialize(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkInstance instance)
{
	VmaVulkanFunctions vmaFuncInfo{};
	VmaAllocatorCreateInfo vmaAllocInfo
	{
		.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
		.physicalDevice = physicalDevice,
		.device = logicalDevice,
		.pVulkanFunctions = &vmaFuncInfo,
		.instance = instance,
		.vulkanApiVersion = VK_MAKE_VERSION(1, 0, 0)
	};

	vmaImportVulkanFunctionsFromVolk(&vmaAllocInfo, &vmaFuncInfo);

	if (vmaCreateAllocator(&vmaAllocInfo, &_allocator) != VK_SUCCESS)
	{
		return false;
	}
	return true;
}
