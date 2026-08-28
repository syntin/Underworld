#include "slangShader.h"

SlangShader::SlangShader()
{

}

SlangShader::~SlangShader()
{

}

void SlangShader::Initialize()
{
	_slang::createGlobalSession(slangGlobalSession.writeRef());
	auto slangTargets{ std::to_array<_slang::TargetDesc>({ {.format{SLANG_SPIRV}, .profile{slangGlobalSession->findProfile("spirv_1_4")} } }) };
	auto slangOptions{ std::to_array<_slang::CompilerOptionEntry>({ { slang::CompilerOptionName::EmitSpirvDirectly, {slang::CompilerOptionValueKind::Int, 1} } }) };
	_slang::SessionDesc slangSessionDesc{
		.targets{slangTargets.data()},
		.targetCount{SlangInt(slangTargets.size())},
		.defaultMatrixLayoutMode = SLANG_MATRIX_LAYOUT_COLUMN_MAJOR,
		.compilerOptionEntries{slangOptions.data()},
		.compilerOptionEntryCount{uint32_t(slangOptions.size())} };
}