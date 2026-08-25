#include "bindlessRender.h"
#include "descriptorPool.h"
#include "utils.h"

BindlessRender::BindlessRender()
{

}

BindlessRender::~BindlessRender()
{

}

VkPhysicalDeviceDescriptorIndexingFeatures BindlessRender::QueryDeviceForBindlessSupport()
{
	VkPhysicalDeviceDescriptorIndexingFeatures indexingFeatures{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES, nullptr };
	VkPhysicalDeviceFeatures2 device_features{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, &indexingFeatures };
	vkGetPhysicalDeviceFeatures2(_vulkanPhysicalDevice, &device_features);
	_bindlessSupported = indexingFeatures.descriptorBindingPartiallyBound && indexingFeatures.runtimeDescriptorArray;
	DescriptorPool pool;
	pool.Create((VkAllocationCallbacks*) &debugCallback);
	return indexingFeatures;
}

void BindlessRender::CreateAndEnableBindlessDevice(VkPhysicalDeviceDescriptorIndexingFeatures indexingFeatures, VkDevice vulkanDevice)
{
	VkPhysicalDeviceFeatures2 physicalFeatures2 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2 };
	vkGetPhysicalDeviceFeatures2(_vulkanPhysicalDevice, &physicalFeatures2);
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.pNext = &physicalFeatures2;
	if (_bindlessSupported) {
		physicalFeatures2.pNext = &indexingFeatures;
	}
	vkCreateDevice(_vulkanPhysicalDevice, &deviceCreateInfo, (const VkAllocationCallbacks*) &debugCallback, &vulkanDevice);
	DescriptorPool descriptorPool{};
	descriptorPool.Create((VkAllocationCallbacks*) &debugCallback);
}
