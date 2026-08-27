#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <GLFW/glfw3.h>

class DebugMessenger
{
public:
	DebugMessenger();
	virtual ~DebugMessenger();

public:
	void Initialize();

private:
	std::vector<const char*> _extensions;
};