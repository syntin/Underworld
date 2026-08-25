#pragma once

#include <vulkan/vulkan.h>
#include <cstdint>

class DescriptorSet
{
public:
	DescriptorSet();
	virtual ~DescriptorSet();

public:
	void LayoutBindings(uint32_t poolSizesBindless, uint32_t kMaxBindlessResources);

private:
};