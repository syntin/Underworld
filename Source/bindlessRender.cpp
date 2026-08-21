#include "bindlessRender.h"
#include "descriptorPool.h"

void BindlessRender::SetAllocationCallbacks(const VkAllocationCallbacks* pCallbacks)
{
	_vkAllocationCallbacks = pCallbacks;
}

VkPhysicalDeviceDescriptorIndexingFeatures BindlessRender::QueryDeviceForBindlessSupport()
{
	VkPhysicalDeviceDescriptorIndexingFeatures indexingFeatures{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES, nullptr };
	VkPhysicalDeviceFeatures2 device_features{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, &indexingFeatures };
	vkGetPhysicalDeviceFeatures2(_vulkanPhysicalDevice, &device_features);
	_isSupported = indexingFeatures.descriptorBindingPartiallyBound && indexingFeatures.runtimeDescriptorArray;
	return indexingFeatures;
}

void BindlessRender::CreateAndEnableBindlessDevice(VkPhysicalDeviceDescriptorIndexingFeatures indexingFeatures, VkDevice vulkanDevice)
{
	VkPhysicalDeviceFeatures2 physicalFeatures2 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2 };
	vkGetPhysicalDeviceFeatures2(_vulkanPhysicalDevice, &physicalFeatures2);
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.pNext = &physicalFeatures2;
	if (_isSupported) {
		physicalFeatures2.pNext = &indexingFeatures;
	}
	vkCreateDevice(_vulkanPhysicalDevice, &deviceCreateInfo, _vkAllocationCallbacks, &vulkanDevice);
	DescriptorPool descriptorPool{};
	descriptorPool.Create();
}
