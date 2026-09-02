#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <iostream>
#include <stdio.h>
#include <cstdint>
#include <vector>
#include "extensions.h"

using namespace std;

Extensions::Extensions()
{

}

Extensions::~Extensions()
{

}

void Extensions::Initialize()
{
	CheckExtensions();
	GetRequiredExtensions();
}

void Extensions::CheckExtensions()
{
	unsigned int extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	// Show this when using DEBUG mode
	#ifndef NDEBUG
		std::cout << "available extensions:\n";
		for (const auto& extension : extensions) {
			std::cout << '\t' << extension.extensionName << '\n';
		}
	#endif
}


void Extensions::GetRequiredExtensions()
{
	_extensions = SDL_Vulkan_GetInstanceExtensions(&_extensionsCount);
}