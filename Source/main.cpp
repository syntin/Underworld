#include <stdio.h>
#include <Windows.h>
#include <WinBase.h>
#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <GLFW/glfw3.h>
#include "utils.h"
#include "vulkanWrapper.h"

#define WIN32_LEAN_AND_MEAN

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

VulkanWrapper wrapper;

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
     // Initialize Vulkan
    wrapper.InitializeVulkan(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
    
    return 0;
}