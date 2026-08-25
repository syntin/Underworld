#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>
#include <cstdint>

class DescriptorPool
{
public:
	DescriptorPool();
	virtual ~DescriptorPool();

public:
	void Create(VkAllocationCallbacks* allocationCallback);

private:
	VkDevice _vulkanDevice = VK_NULL_HANDLE;
	uint32_t _kMaxBindlessResources = 0;
};