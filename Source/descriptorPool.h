#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

class DescriptorPool
{
public:
	DescriptorPool();
	virtual ~DescriptorPool();

public:
	void Create(VkDevice vulkanDevice);
};