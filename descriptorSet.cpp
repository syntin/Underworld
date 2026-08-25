#include "descriptorSet.h"

DescriptorSet::DescriptorSet()
{

}

DescriptorSet::~DescriptorSet()
{

}

void DescriptorSet::LayoutBindings(uint32_t poolSizesBindless, uint32_t kMaxBindlessResources)
{
	const int32_t pool_count = poolSizesBindless;
	uint32_t bindlessTextureBinding = 0;
	VkDescriptorSetLayoutBinding vkBinding[4];
	VkDescriptorSetLayoutBinding& imageSamplerBinding = vkBinding[0];
	VkDescriptorSetLayoutBinding& storageImageBinding = vkBinding[1];
	imageSamplerBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	imageSamplerBinding.descriptorCount = kMaxBindlessResources;
	imageSamplerBinding.binding = bindlessTextureBinding;
	//storageImageBinding
}
