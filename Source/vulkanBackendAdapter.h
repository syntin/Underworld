//DB
#pragma once
#include "vulkanWrapper.h"

class VulkanBackendAdapter : public VulkanWrapper
{
public:
    void Initialize(SDL_Window* window)
    {
        InitializeVulkan(window);
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