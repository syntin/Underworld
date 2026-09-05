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

class VulkanWrapper
{
public:
	VulkanWrapper();
	virtual ~VulkanWrapper();

public:
	void InitializeVulkan(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
	void RunRenderLoop();
	VkInstance GetInstance() { return _vulkanInstance.GetInstance(); }
	VulkanInstance GetInstanceObject() { return _vulkanInstance; }

protected:
	void Destroy();

private:
	VulkanInstance _vulkanInstance;
	VulkanQueue _queue;
	DebugMessenger _debugMessenger{};
	Extensions _extensions{};
	Window _window{};
	Device _device;
	SwapChain _swapChain;
	DepthAttachment _depthAttachment;
	Mesh _mesh;
	VertexData _vertexData;
	ShaderData _shaderData;
	Synchronization _synchronization;
	CommandPool _commandPool;
	TextureImages _textureImages;
	Descriptor _descriptor;
	SlangShader _slangShader;
	LoadShader _loadShader;
	Pipeline _pipeline;

private:
	VkImageMemoryBarrier2 _imageMemoryBarrier2[2];
};
