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

	printf("Surface::Create — window=%p  flags=0lx%lx\n",
		(void*)window, SDL_GetWindowFlags(window));

	if (!SDL_Vulkan_CreateSurface(window, instance, nullptr, &_surface))
	{
		printf("SDL_Vulkan_CreateSurface failed: %s\n", SDL_GetError());
		return false;
	}

	printf("Surface created: %p\n", (void*)_surface);

	return true;
}