// Author: RC

#pragma once

#include <volk/volk.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>


class Surface
{
public:
	Surface();
	virtual ~Surface();

public:
	bool Create(VkInstance& instance, SDL_Window* window);
	VkSurfaceKHR GetSurface() { return _surface; }
	Surface* Data()
	{
		return this;
	}

private:
	VkSurfaceKHR _surface = nullptr;
};