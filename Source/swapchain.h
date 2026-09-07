#pragma once

#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include "surface.h"
#include "device.h"
#include "vma.h"

class SwapChain
{
public:
	SwapChain();
	virtual ~SwapChain();

public:
	bool Create(Device device, Surface surface, Vma vma, SDL_Window* window, uint32_t width, uint32_t height);
	void Destroy(Device device, Vma vma);

public:
	uint32_t GetWidth() { return _swapchainWidth; }
	uint32_t GetHeight() { return _swapchainHeight; }
	VkFormat GetSwapChainFormat() { return _swapchainFormat; }
	VkSwapchainKHR GetSwapChain() { return _swapchain; }
	std::vector<VkImage> GetSwapChainImages() { return _swapchainImages; }
	std::vector<VkImageView> GetSwapChainImageViews() { return _swapchainImageViews; }
	std::vector<VkSemaphore> GetRenderCompleteSemaphores() { _renderCompleteSemaphores; }
	bool GetSwapChainRecreate() { return _requireSwapchainRecreate; }
	VkImage GetDepthImage() { return _depthImage; }
	VkImageView GetDepthImageView() { return _depthImageView; }
	VmaAllocation GetDepthImageAllocation() { return _depthImageAllocation; }

protected:
	bool CreateInfoKHR(Device device, Surface surface);
	bool CreateImageView(Device device);
	bool RenderCompleteSemaphores(Device device);
	bool CreateDepthImageView(Device device, Vma vma);

private:
	SDL_Window* _window = nullptr;
	uint32_t _swapchainWidth = 0;
	uint32_t _swapchainHeight = 0;
	VkFormat _swapchainFormat = VK_FORMAT_B8G8R8A8_SRGB;
	VkSwapchainKHR _swapchain = nullptr;
	std::vector<VkImage> _swapchainImages;
	std::vector<VkImageView> _swapchainImageViews;
	std::vector<VkSemaphore> _renderCompleteSemaphores;
	bool _requireSwapchainRecreate = false;
	VkImage _depthImage = nullptr;
	VkImageView _depthImageView = nullptr;
	VmaAllocation _depthImageAllocation = nullptr;
};

