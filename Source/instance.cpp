#include "instance.h"
#include "utils.h"

VulkanInstance::VulkanInstance()
{

}

VulkanInstance::~VulkanInstance()
{

}

void VulkanInstance::CreateInstance()
{
	VkInstanceCreateInfo createInfo{};

	vkCreateInstance(&createInfo, nullptr, &_instance);
}

void VulkanInstance::DestroyInstance()
{
	if (_instance)
	{
		vkDestroyInstance(_instance, nullptr);
	}
}