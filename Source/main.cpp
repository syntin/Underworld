
#include <Windows.h>
#include <stdio.h>
#include <WinBase.h>

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 0
#define VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#define VK_NO_PROTOTYPES
#include <volk/volk.h>
#define SDL_IMPLEMENTATION
#include <SDL3/SDL.h>
#define GLFW_IMPLEMENTATION
#include <GLFW/glfw3.h>
#define VMA_IMPLEMENTATION
#define STB_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#include "utils.h"
#include "vulkanWrapper.h"

#pragma warning(disable:4005)

#define WIN32_LEAN_AND_MEAN

#undef APIENTRY
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
    //wrapper.RunRenderLoop();
    
    return 0;
}