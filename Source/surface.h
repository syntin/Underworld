#pragma once

class Surface
{
public:
	Surface();
	virtual ~Surface();

public:
	bool Create(VkInstance instance, SDL_Window* window);
};