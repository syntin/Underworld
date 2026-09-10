// Author: RC & DB

#include "shader.h"
#include "utils.h"

#include <Volk/volk.h>
#include <SDL3/SDL.h>
#ifdef VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>
#endif

#include <iostream>
#include <fstream>

// Helper to read file into string
static std::string ReadTextFile(const std::string& path)
{
	std::ifstream file(path);
	if (!file.is_open())
		return "";

	return std::string((std::istreambuf_iterator<char>(file)),
						std::istreambuf_iterator<char>());
}

Shader::Shader()
{

}

Shader::~Shader()
{

}

VkShaderModule Shader::CreateShaderModule(Device& device, const std::string& fileName, shaderc_shader_kind kind) const
{
	// Load source
	const std::string shaderPath = "src/shaders/" + fileName;
	const std::string src = ReadTextFile(shaderPath);

	if (src.empty())
	{
		showError("Specified shader file doesn't exist: " + shaderPath, nullptr);
		return nullptr;
	}

	// compile the shader to SPIR-V
	std::cout << "Compiling shader: " << shaderPath << std::endl;
	shaderc::Compiler compiler;
	shaderc::CompileOptions opts;

	opts.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
	opts.SetOptimizationLevel(shaderc_optimization_level_performance);

	shaderc::CompilationResult result = compiler.CompileGlslToSpv(src, kind, fileName.c_str(), opts);

	if (result.GetCompilationStatus() != shaderc_compilation_status_success)
	{
		std::cerr << "Shader Compilation Error: " << result.GetErrorMessage() << std::endl;
		return VK_NULL_HANDLE;
	}

	std::vector<uint32_t> spv = { result.cbegin(), result.cend() };

	// pass spir-v to vulkan and create shader-module
	VkShaderModuleCreateInfo moduleCreateInfo{};
	moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	moduleCreateInfo.codeSize = spv.size() * sizeof(uint32_t);
	moduleCreateInfo.pCode = spv.data();
	
	VkShaderModule shaderModule = VK_NULL_HANDLE;
	if (vkCreateShaderModule(device.GetLogicalDevice(), &moduleCreateInfo, nullptr, &shaderModule) != VK_SUCCESS)
	{
		showError("Error creating shader module", nullptr);
		return VK_NULL_HANDLE;
	}

	return shaderModule;
}

bool Shader::Create(Device& device)
{
	_vertShader = CreateShaderModule(device, "shader.vert", shaderc_vertex_shader);
	if (_vertShader == VK_NULL_HANDLE)
		return false;

	_fragShader = CreateShaderModule(device, "shader.frag", shaderc_fragment_shader);
	if (_fragShader == VK_NULL_HANDLE)
		return false;

	return true;
}