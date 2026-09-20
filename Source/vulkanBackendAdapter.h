//DB
#pragma once
#include "vulkanWrapper.h"

class VulkanBackendAdapter : public VulkanWrapper
{
public:
	void Initialize()
	{
		InitializeVulkan(GetModuleHandle(nullptr), nullptr, nullptr, SW_SHOW);
	}

	void RenderFrame()
	{
		Render();
	}

	void SetWorld(World* world)
	{
		VulkanWrapper::SetWorld(world);
	}
};