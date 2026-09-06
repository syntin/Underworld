#include "surface.h"
#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>

Surface::Surface()
{
}

Surface::~Surface()
{
}

bool Surface::Create(VkInstance instance, SDL_Window* window)
{
	VkSurfaceKHR surface;
	if (!SDL_Vulkan_CreateSurface(window, instance, &surface))
	{
		showError("Failed to create Vulkan surface", window);
		return false;
	}
	return true;
}