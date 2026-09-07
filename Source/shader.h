// Author: RC

#pragma once

#define VK_NO_PROTOTYPES
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <array>
#include <string>
#include <shaderc/shaderc.hpp>
#include "device.h"


class Shader
{
public:
	Shader();
	virtual ~Shader();

public:
	bool Create(Device device);
	VkShaderModule GetVertShader() { return _vertShader; }
	VkShaderModule GetFragShader() { return _fragShader; }

protected:
	VkShaderModule CreateShaderModule(VkDevice device, const std::string& fileName, shaderc_shader_kind kind) const;

private:
	VkShaderModule _vertShader{ VK_NULL_HANDLE };
	VkShaderModule _fragShader{ VK_NULL_HANDLE };
};