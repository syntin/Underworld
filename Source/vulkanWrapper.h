#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include "window.h"
#include "debugMessenger.h"
#include "instance.h"
#include "validationLayer.h"
#include "extensions.h"
#include "vulkanQueue.h"
#include "swapchain.h"
#include "depthAttachment.h"
#include "mesh.h"
#include "vertexData.h"
#include "shaderData.h"
#include "synchronization.h"
#include "commandPool.h"
#include "textureImages.h"
#include "descriptor.h"
#include "slangShader.h"
#include "loadShader.h"
#include "pipeline.h"
#include "volkLoader.h"
#include "instance.h"
#include "extensions.h"
#include "surface.h"
#include "vma.h"
#include "graphicsQueue.h"

class VulkanWrapper
{
public:
	VulkanWrapper();
	virtual ~VulkanWrapper();

public:
	void InitializeVulkan(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
	void Run();
	void Render();
	bool InitializeVulkan();

protected:
	void Destroy();

private:
	VulkanInstance _instance{};
	Window _window{};
	Surface _surface{};
	Device _physicalDevice{};
	GraphicsQueue _graphicsQueue{};
	Vma _vma{};
	SwapChain _swapChain{};
	ShaderData _shaderData{};
	Pipeline _pipeline{};
	Synchronization _synchronization{};
	CommandPool _commandPool{};

/*
	VolkLoader _volkLoader{};
	Extensions _extensions{};
	DebugMessenger _debugMessenger{};
	Extensions _extensions{};
	DepthAttachment _depthAttachment;
	Mesh _mesh;
	VertexData _vertexData;
	TextureImages _textureImages;
	Descriptor _descriptor;
	SlangShader _slangShader;
	LoadShader _loadShader;
*/

private:
	VkImageMemoryBarrier2 _imageMemoryBarrier2[2];
	bool _running = true;
	//VkDevice _vulkanDevice = nullptr;
	//VkDevice _device = nullptr;
};
