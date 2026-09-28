// Author: RC

#include "surface.h"
#include <stdio.h>

Surface::Surface()
{
}

Surface::~Surface()
{
}

bool Surface::Create(VkInstance& instance, SDL_Window* window)
{
	_surface = VK_NULL_HANDLE;

	

	if (!SDL_Vulkan_CreateSurface(window, instance, nullptr, &_surface))
	{
		printf("SDL_Vulkan_CreateSurface failed: %s\n", SDL_GetError());
		return false;
	}

	

	return true;
}