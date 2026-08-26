#include <stdio.h>
#include <Windows.h>
#include <WinBase.h>
#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <GLFW/glfw3.h>
#include "vulkanWrapper.h"

#pragma warning(disable: 4005)
#pragma warning(disable: 4098)

#define WIN32_LEAN_AND_MEAN

VulkanWrapper vkWrapper;

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	vkWrapper.InitializeVulkan(hInstance, hPrevInstance, lpCmdLine, nShowCmd);

	return 0;
}

