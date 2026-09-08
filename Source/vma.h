// Author: RC

#pragma once

#include <Volk/volk.h>
#include <vma/vk_mem_alloc.h>
#include "device.h"
#include "instance.h"

class Vma
{
public:
	Vma();
	virtual ~Vma();

public:
	bool Initialize(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkInstance instance);
	VmaAllocator GetAllocator() { return _allocator; }

private:
	VmaAllocator _allocator = nullptr;
};