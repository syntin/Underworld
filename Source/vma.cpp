#include "vma.h"
#include "utils.h"

Vma::Vma()
{

}

Vma::~Vma()
{

}

Vma::Initialize()
{
	VmaVulkanFunctions vkFunctions{
		.vkGetInstanceProcAddr = vkGetInstanceProcAddr,
		.vkGetDeviceProcAddr = vkGetDeviceProcAddr,
		.vkCreateImage = vkCreateImage
	};
	VmaAllocatorCreateInfo allocatorCI{
		.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
		.physicalDevice = devices[deviceIndex],
		.device = device,
		.pVulkanFunctions = &vkFunctions,
		.instance = instance
	};
	chk(vmaCreateAllocator(&allocatorCI, &allocator));
}