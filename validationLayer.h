#pragma once
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vector>

class ValidationLayer
{
public:
	ValidationLayer();
	virtual ~ValidationLayer();

public:
	bool CheckValidationLayerSupport();

private:
	const std::vector<const char*> _validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

};