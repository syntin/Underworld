//  Author: RC

#include "swapchain.h"
#include <Volk/volk.h>
#include <vma/vk_mem_alloc.h>
#include "device.h"
#include "utils.h"
#include <stdio.h>

SwapChain::SwapChain()
{

}

SwapChain::~SwapChain()
{

}

bool SwapChain::CreateInfoKHR(Device& device, Surface& surface)
{
	VkSurfaceCapabilitiesKHR surfaceCaps{};
	VkPhysicalDevice physicalDevice = device.GetPhysicalDevice();
	VkSurfaceKHR surf = surface.GetSurface();

	printf("Capabilities query: phys=%p  surf=%p\n",
		(void*)physicalDevice, (void*)surf);

	VkResult res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surf, &surfaceCaps);
	printf("vkGetPhysicalDeviceSurfaceCapabilitiesKHR result: %d\n", res);

	if (res == VK_ERROR_SURFACE_LOST_KHR)
	{
		printf("SURFACE LOST — shit is cooked\n");
		return false; 
	}

	if (res != VK_SUCCESS)
	{
		showError("Couldn't get the surface capabilities", nullptr);
		return false;
	}

	uint32_t imageCount = surfaceCaps.minImageCount + 1;
	if (surfaceCaps.maxImageCount > 0 && imageCount > surfaceCaps.maxImageCount)
		imageCount = surfaceCaps.maxImageCount;

	VkExtent2D extent = { _swapchainWidth, _swapchainHeight };
	if (surfaceCaps.currentExtent.width != UINT32_MAX)
	{
		extent = surfaceCaps.currentExtent;
	}

	VkSwapchainCreateInfoKHR swapchainCreateInfo{
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = surf,
		.minImageCount = imageCount,
		.imageFormat = _swapchainFormat,
		.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR,
		.imageExtent = extent,
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.preTransform = surfaceCaps.currentTransform,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = VK_PRESENT_MODE_FIFO_KHR,
		.clipped = VK_TRUE,
		.oldSwapchain = VK_NULL_HANDLE
	};

	if (vkCreateSwapchainKHR(device.GetLogicalDevice(), &swapchainCreateInfo, nullptr, &_swapchain) != VK_SUCCESS)
	{
		showError("Error creating swapchain", nullptr);
		return false;
	}

	return true;
}

bool SwapChain::CreateImageView(Device device)
{
	uint32_t imageCount = 0;
	vkGetSwapchainImagesKHR(device.GetLogicalDevice(), _swapchain, &imageCount, nullptr);
	_swapchainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device.GetLogicalDevice(), _swapchain, &imageCount, _swapchainImages.data());
	_swapchainImageViews.resize(imageCount);

	for (size_t i = 0; i < _swapchainImages.size(); ++i)
	{
		VkImageViewCreateInfo imgViewInfo
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = _swapchainImages[i],
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = _swapchainFormat,
			.subresourceRange
			{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.levelCount = 1,
				.layerCount = 1
			}
		};

		if (vkCreateImageView(device.GetLogicalDevice(), &imgViewInfo, nullptr, &_swapchainImageViews[i]) != VK_SUCCESS)
		{
			showError("Error creating swapchain image view", nullptr);
			return false;
		}
	}
	return true;
}

bool SwapChain::RenderCompleteSemaphores(Device device)
{
	_renderCompleteSemaphores.resize(_swapchainImages.size());
	for (VkSemaphore& semaphore : _renderCompleteSemaphores)
	{
		VkSemaphoreCreateInfo semaphoreInfo{ .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
		if (vkCreateSemaphore(device.GetLogicalDevice(), &semaphoreInfo, nullptr, &semaphore) != VK_SUCCESS)
		{
			showError("Error creating the render-complete semaphore", nullptr);
			return false;
		}
	}
	return true;
}

bool SwapChain::CreateDepthImageView(Device device, Vma vma)
{
	VkImageCreateInfo depthCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.imageType = VK_IMAGE_TYPE_2D,
		.format = depthFormat,
		.extent{.width = _swapchainWidth, .height = _swapchainHeight, .depth = 1 },
		.mipLevels = 1,
		.arrayLayers = 1,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.tiling = VK_IMAGE_TILING_OPTIMAL,
		.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
	};

	VmaAllocationCreateInfo allocInfo
	{
		.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
		.usage = VMA_MEMORY_USAGE_AUTO
	};
	if (vmaCreateImage(vma.GetAllocator(), &depthCreateInfo, &allocInfo, &_depthImage, &_depthImageAllocation, nullptr) != VK_SUCCESS)
	{
		showError("Error allocating depth image", nullptr);
		return false;
	}

	VkImageViewCreateInfo depthImgViewInfo
	{
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.image = _depthImage,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = depthFormat,
		.subresourceRange{.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT, .levelCount = 1, .layerCount = 1}
	};

	if (vkCreateImageView(device.GetLogicalDevice(), &depthImgViewInfo, nullptr, &_depthImageView) != VK_SUCCESS)
	{
		showError("Error creating depth image view", nullptr);
		return false;
	}
	return true;
}

bool SwapChain::Create(Device& device, Surface& surface, Vma& vma,
					   SDL_Window* window, uint32_t width, uint32_t height)
{
	_window = window;
	_swapchainWidth = width;
	_swapchainHeight = height;
	_swapchainFormat = VK_FORMAT_B8G8R8A8_SRGB; 


	if (!CreateInfoKHR(device, surface))
	{
		showError("Error creating swapchain info", _window);
		return false;
	}


	if (!CreateImageView(device))
	{
		showError("Error creating swapchain image views", _window);
		return false;
	}

	if (!RenderCompleteSemaphores(device))
	{
		showError("Error creating render-complete semaphores", _window);
		return false;
	}

	depthFormat = VK_FORMAT_D32_SFLOAT;

	if (!CreateDepthImageView(device, vma))
	{
		showError("Error creating depth image view", _window);
		return false;
	}

	return true;
}

void SwapChain::Destroy(Device device, Vma vma)
{
	for (VkImageView swapchainImgView : _swapchainImageViews)
	{
		vkDestroyImageView(device.GetLogicalDevice(), swapchainImgView, nullptr);
	}
	_swapchainImageViews.clear();

	// destroy render-complete ssemaphores
	for (VkSemaphore& semaphore : _renderCompleteSemaphores)
	{
		vkDestroySemaphore(device.GetLogicalDevice(), semaphore, nullptr);
		semaphore = nullptr;
	}
	_renderCompleteSemaphores.clear();

	if (_swapchain)
	{
		vkDestroySwapchainKHR(device.GetLogicalDevice(), _swapchain, nullptr);
		_swapchain = nullptr;
	}

	// destroy the depth buffer along with the swapchain
	if (_depthImageView)
	{
		vkDestroyImageView(device.GetLogicalDevice(), _depthImageView, nullptr);
		vmaDestroyImage(vma.GetAllocator(), _depthImage, _depthImageAllocation);
		_depthImageView = nullptr;
	}
}

