#pragma once

#include "device.h"
#include "instance.h"

class Vma
{
public:
	Vma();
	virtual ~Vma();

public:
	bool Initialize(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkInstance instance);

private:
	VmaAllocator _allocator = nullptr;
};