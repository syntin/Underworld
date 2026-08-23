#include "bindlessRender.h"
#include "descriptorPool.h"

BindlessRender::BindlessRender()
{

}

BindlessRender::~BindlessRender()
{

}

void BindlessRender::SetAllocationCallbacks(const VkAllocationCallbacks* pCallbacks)
{
	_vkAllocationCallback = pCallbacks;
}

VkPhysicalDeviceDescriptorIndexingFeatures BindlessRender::QueryDeviceForBindlessSupport()
{
	VkPhysicalDeviceDescriptorIndexingFeatures indexingFeatures{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES, nullptr };
	VkPhysicalDeviceFeatures2 device_features{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, &indexingFeatures };
	vkGetPhysicalDeviceFeatures2(_vulkanPhysicalDevice, &device_features);
	_bindlessSupported = indexingFeatures.descriptorBindingPartiallyBound && indexingFeatures.runtimeDescriptorArray;
	DescriptorPool pool;
	pool.Create(_vkAllocationCallback);
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
	vkCreateDevice(_vulkanPhysicalDevice, &deviceCreateInfo, &_vkAllocationCallback, &vulkanDevice);
	DescriptorPool descriptorPool{};
	descriptorPool.Create(&_vkAllocationCallback);
}
