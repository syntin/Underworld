#pragma once

#include <vulkan/vulkan.h>

class DescriptorPool
{
public:
	void DescriptorPool();
	virtual ~DescriptorPool();

public:
	void Create();
};