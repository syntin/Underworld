#pragma once

#include <vulkan/vulkan.h>
//#define GLFW_DLL
//#include <GLFW/glfw3.h>
#include <GLFW/glfw3.h>


class Engine
{
public:
	Engine();
	virtual ~Engine();

public:
	void Initialize();
};


