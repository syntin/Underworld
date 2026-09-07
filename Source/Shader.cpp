// Author: RC

#include "shader.h"
#include "utils.h"

#include <SDL3/SDL.h>
#ifndef VK_NO_PROTOTYPES
	#define VK_NO_PROTOTYPES
#endif
#include <vulkan/vulkan.h>
#ifdef VOLK_IMPLEMENTATION
	#include <Volk/volk.h>
#endif
#ifdef VMA_IMPLEMENTATION
	#include <vma/vk_mem_alloc.h>
#endif

#include <iostream>
#include <fstream>
#include <string>


Shader::Shader()
{

}

Shader::~Shader()
{

}

VkShaderModule Shader::CreateShaderModule(VkDevice device, const std::string& fileName, shaderc_shader_kind kind) const
{
	const std::string shaderPath = "src/shaders/" + fileName;
	const std::string src = readTextFile(shaderPath);
	if (src.empty())
	{
		showError("Specified shader file doesn't exist: " + shaderPath);
		return nullptr;
	}

	// compile the shader to SPIR-V
	std::cout << "Compiling shader: " << shaderPath << std::endl;
	shaderc::Compiler compiler;
	shaderc::CompileOptions opts;
	opts.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_4);
	opts.SetTargetSpirv(shaderc_spirv_version_1_6);
	opts.SetOptimizationLevel(shaderc_optimization_level_performance);
	shaderc::CompilationResult result = compiler.CompileGlslToSpv(src, kind, fileName.c_str(), opts);

	if (result.GetCompilationStatus() != shaderc_compilation_status_success)
	{
		std::cerr << "Shader Compilation Error: " << result.GetErrorMessage() << std::endl;
		return nullptr;
	}
	std::vector<uint32_t> spv = { result.cbegin(), result.cend() };

	// pass spir-v to vulkan and create shader-module
	VkShaderModuleCreateInfo moduleCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = spv.size() * sizeof(uint32_t),
		.pCode = spv.data()
	};
	VkShaderModule shaderModule = nullptr;
	if (vkCreateShaderModule(device, &moduleCreateInfo, nullptr, &shaderModule) != VK_SUCCESS)
	{
		showError("Error creating shader module");
		return nullptr;
	}
	return shaderModule;
}

bool Shader::Create(Device device)
{
	if (_vertShader = CreateShaderModule(device.GetLogicalDevice(), "shader.vert", shaderc_vertex_shader); shaderc_vertex_shader)
	{
		return false;
	}
	if (_fragShader = CreateShaderModule(device.GetLogicalDevice(), "shader.frag", shaderc_fragment_shader); shaderc_fragment_shader)
	{
		return false;
	}
	return true;
}