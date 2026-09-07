// Author: RC

#include "surface.h"

Surface::Surface()
{
}

Surface::~Surface()
{
}

bool Surface::Create(VkInstance& instance, SDL_Window* window)
{
	if (!SDL_Vulkan_CreateSurface(window, instance, nullptr, &_surface))
	{
		return false;
	}
	return true;
}