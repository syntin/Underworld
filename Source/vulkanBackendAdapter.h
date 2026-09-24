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

	void OnWindowResize(uint32_t width, uint32_t height)
	{
		RequestSwapChainRecreate(width, height);
	}
};