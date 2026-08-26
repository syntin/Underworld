#pragma once
#include <vulkan/vulkan.h>

class VulkanInstance
{
public:
	VulkanInstance();
	virtual ~VulkanInstance();

public:
	void CreateInstance();
	void DestroyInstance();

private:
	bool CheckValidationLayerSupport();

private:
	VkInstance _instance;
};