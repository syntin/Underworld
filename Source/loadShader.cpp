#include "loadShader.h"

LoadShader::LoadShader()
{

}

LoadShader::~LoadShader()
{

}

void LoadShader::Initialize()
{
	Slang::ComPtr<slang::ISession> slangSession;
	slangGlobalSession->createSession(slangSessionDesc, slangSession.writeRef());
	Slang::ComPtr<slang::IModule> slangModule{ slangSession->loadModuleFromSource("triangle", "assets/shader.slang", nullptr, nullptr) };
	Slang::ComPtr<ISlangBlob> spirv;
	slangModule->getTargetCode(0, spirv.writeRef());
	VkShaderModuleCreateInfo shaderModuleCI{ .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO, .codeSize = spirv->getBufferSize(), .pCode = (uint32_t*)spirv->getBufferPointer() };
	VkShaderModule shaderModule{};
	chk(vkCreateShaderModule(device, &shaderModuleCI, nullptr, &shaderModule));
}