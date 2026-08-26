#pragma once

class VulkanQueue
{
public:
	VulkanQueue();
	virtual ~VulkanQueue();

public:
	void Initialize(VkInstance instance);
};