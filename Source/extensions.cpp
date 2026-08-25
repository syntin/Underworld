#include "extensions.h"
#include <GLFW/glfw3.h>>
#include <vulkan/vulkan.h>
#include <iostream>
#include <cstdint>
#include <vector>

using namespace std;

Extensions::Extensions()
{

}

Extensions::~Extensions()
{

}

void Extensions::GetRequiredExtensions()
{

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


void Extensions::GetRequiredExtensions() {
	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	_extensions = vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
		_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
}