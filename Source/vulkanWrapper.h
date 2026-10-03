// Author: RC

#pragma once

#include <Windows.h>
#include <Volk/volk.h>
#include <VMA/vk_mem_alloc.h>
//#include <GLFW/glfw3.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include "window.h"
#include "debugMessenger.h"
#include "instance.h"
#include "validationLayer.h"
#include "extensions.h"
#include "graphicsQueue.h"
#include "swapchain.h"
#include "depthAttachment.h"
#include "mesh.h"
//#include "vertex.h"
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

class World;

class VulkanWrapper
{
public:
	VulkanWrapper();
	virtual ~VulkanWrapper();

public:
	void InitializeVulkan(SDL_Window* existingWindow);
	void Run();

	void SetWorld(World* world)
	{
		_world = world;
		_bindlessRender.SetWorld(world);
	}

	Window* GetWindow()
	{
		return &_window;
	}


	void RequestSwapChainRecreate(uint32_t width, uint32_t height);
	bool IsRunning() const { return _running; }

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
	World* _world = nullptr;

private:
	VkImageMemoryBarrier2 _imageMemoryBarrier2[2];
	bool _running = true;
	uint32_t _newWidth = WIDTH;
	uint32_t _newHeight = HEIGHT;
	uint32_t _requestedWidth = 0;
	uint32_t _requestedHeight = 0;
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
