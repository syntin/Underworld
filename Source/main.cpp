#include <Windows.h>
#include <stdio.h>
#include <WinBase.h>
#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <GLFW/glfw3.h>
#include "utils.h"
#include "vulkanWrapper.h"

#define WIN32_LEAN_AND_MEAN

#ifndef APIENTRY
    #ifdef _WIN32
        #define APIENTRY __stdcall
    #else
        #define APIENTRY
    #endif
    #define GL_APIENTRY_DEFINED
#endif // APIENTRY

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    VulkanWrapper wrapper;

    // Initialize Vulkan
    wrapper.InitializeVulkan(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
    wrapper.RunRenderLoop();
    
    return 0;
}