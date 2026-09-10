// Author: RC

#pragma once

#include <Volk/volk.h>
#include <vma/vk_mem_alloc.h>
#include <GLFW/glfw3.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include "window.h"
#include "debugMessenger.h"
#include "instance.h"
#include "validationLayer.h"
#include "extensions.h"
//#include "vulkanQueue.h"
#include "graphicsQueue.h"
#include "swapchain.h"
#include "depthAttachment.h"
#include "mesh.h"
//#include "vertexData.h"
#include "shader.h"
#include "synchronization.h"
#include "commandBuffer.h"
#include "textureImages.h"
#include "descriptor.h"
#include "slangShader.h"
#include "graphicsPipeline.h"
#include "volkLoader.h"
#include "instance.h"
#include "extensions.h"
#include "surface.h"
#include "vma.h"
#include "graphicsQueue.h"
#include "bindlessRender.h"
#include "device.h"
#include "semaphores.h"


class VulkanWrapper
{
public:
	VulkanWrapper();
	virtual ~VulkanWrapper();

public:
	void InitializeVulkan(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
	void Run();

protected:
	bool SetupVulkan();
	void Render();
	void Destroy();
	void DestroySwapchain();

private:
	VulkanInstance _instance{};
	Window _window{};
	Surface _surface{};
	Device _device{};
	GraphicsQueue _graphicsQueue{};
	Vma _vma{};
	SwapChain _swapChain{};
	Shader _shader{};
	GraphicsPipeline _pipeline{};
	Synchronization _synchronization{};
	CommandBuffer _commandBuffer{};
	BindlessRender _bindlessRender{};
	Semaphore _semaphores;

private:
	VkImageMemoryBarrier2 _imageMemoryBarrier2[2];
	bool _running = true;
	uint32_t _newWidth = WIDTH;
	uint32_t _newHeight = HEIGHT;
};


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
