// Author: RC

#include <volk/volk.h>
#include "bindlessRender.h"
#include "descriptorPool.h"
#include "utils.h"

BindlessRender::BindlessRender()
{

}

BindlessRender::~BindlessRender()
{

}

void BindlessRender::Initialize(
	Device* device,
	SwapChain* swapChain,
	GraphicsQueue* graphicsQueue,
	Synchronization* sync,
	CommandBuffer* cmdBuffer,
	GraphicsPipeline* pipeline)
{
	_device = device;
	_swapChain = swapChain;
	_graphicsQueue = graphicsQueue;
	_sync = sync;
	_cmdBuffer = cmdBuffer;
	_pipeline = pipeline;
}

VkPhysicalDeviceDescriptorIndexingFeatures BindlessRender::QueryDeviceForBindlessSupport()
{
	VkPhysicalDeviceDescriptorIndexingFeatures indexingFeatures{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES, nullptr };
	VkPhysicalDeviceFeatures2 device_features{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, &indexingFeatures };
	vkGetPhysicalDeviceFeatures2(_vulkanPhysicalDevice, &device_features);
	_bindlessSupported = indexingFeatures.descriptorBindingPartiallyBound && indexingFeatures.runtimeDescriptorArray;
	DescriptorPool pool;
	pool.Create((VkAllocationCallbacks*) nullptr);// &debugCallback);
	return indexingFeatures;
}

void BindlessRender::CreateAndEnableBindlessDevice(VkPhysicalDeviceDescriptorIndexingFeatures indexingFeatures, VkDevice vulkanDevice)
{
	VkPhysicalDeviceFeatures2 physicalFeatures2 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2 };
	vkGetPhysicalDeviceFeatures2(_vulkanPhysicalDevice, &physicalFeatures2);
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.pNext = &physicalFeatures2;
	if (_bindlessSupported) {
		physicalFeatures2.pNext = &indexingFeatures;
	}
	vkCreateDevice(_vulkanPhysicalDevice, &deviceCreateInfo, (const VkAllocationCallbacks*) /*&debugCallback*/ nullptr, &vulkanDevice);
	DescriptorPool descriptorPool{};
	descriptorPool.Create((VkAllocationCallbacks*) /*&debugCallback*/ nullptr);

	_bindlessEnabled = _bindlessSupported;
}

void BindlessRender::Render()
{
	std::cout << "BindlessRender::Render begin" << std::endl;

	if (!_device || !_swapChain || !_graphicsQueue || !_sync || !_pipeline)
		return;

	auto& frameResources = _sync->GetFrameResources();
	auto& renderCompleteSemaphores = _swapChain->GetRenderCompleteSemaphores();

	VkSemaphore timelineSemaphore = _sync->GetTimelineSemaphore();
	VkDevice device = _device->GetLogicalDevice();
	VkSwapchainKHR swapchain = _swapChain->GetSwapChain();

	auto swapchainImages = _swapChain->GetSwapChainImages();
	auto swapchainImageViews = _swapChain->GetSwapChainImageViews();

	VkImage depthImage = _swapChain->GetDepthImage();
	VkImageView depthImageView = _swapChain->GetDepthImageView();

	uint32_t swapchainWidth = _swapChain->GetWidth();
	uint32_t swapchainHeight = _swapChain->GetHeight();

	VkQueue gfxQueue = *_graphicsQueue->GetGraphicsQueue();


	// first check if our swapchain is still valid
	//if (requireSwapchainRecreate)
	//{
	//	vkDeviceWaitIdle(device);
	//	destroySwapchain();
	//	createSwapchain(width, height);
	//	requireSwapchainRecreate = false;
	//}

	const uint32_t frameResIndex = _frameIndex++ % MaxFramesInFlight;
	const uint64_t signalValue = ++_nextSignalValue;

	if (signalValue > MaxFramesInFlight)
	{
		const uint64_t waitValue = signalValue - MaxFramesInFlight;

		VkSemaphoreWaitInfo waitInfo{
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO,
			.semaphoreCount = 1,
			.pSemaphores = &timelineSemaphore,
			.pValues = &waitValue
		};
		vkWaitSemaphores(device, &waitInfo, UINT64_MAX);
	}

	// now its safe to start recording commands
	FrameResources& res = frameResources[frameResIndex];
	//vkResetCommandPool(device, res._commandPool, 0);

	// get the resources for this frame
	VkSemaphore imageAcquireSemaphore = res._imageAcquiredSemaphore;

	uint32_t imageIndex = 0;
	VkResult acquireResult = vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, imageAcquireSemaphore, VK_NULL_HANDLE, &imageIndex);



	// handle resize and out-of-date images, may need swapchain recreate
	//if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR)
	//{
	//	_swapChain->SetSwapChainRecreate(true);
	//	return;
	//}
	//else if (acquireResult == VK_SUBOPTIMAL_KHR)
	//{
	//	// can render this frame, recreate next time around
	//	// requireSwapchainRecreate = true;
	//	_swapChain->SetSwapChainRecreate(true);
	//}

	VkCommandBuffer cmd = res._commandBuffer;

	// begin recording commands
	VkCommandBufferBeginInfo cmdBeginInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
	};
	vkBeginCommandBuffer(cmd, &cmdBeginInfo);

	// transition the color and depth images
	std::vector<VkImageMemoryBarrier2> layoutBarriers
	{
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
			.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
			.srcAccessMask = 0,
			.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
			.dstAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
			.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			.image = swapchainImages[imageIndex],
			.subresourceRange
			{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			}
		},
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
			.srcStageMask = VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT,
			.srcAccessMask = 0,
			.dstStageMask = VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT, // both specified to control memory access at both stages (write)
			.dstAccessMask = VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
			.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.newLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
			.image = depthImage,
			.subresourceRange
			{
				.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			}
		}
	};
	VkDependencyInfo depInfo
	{
		.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
		.imageMemoryBarrierCount = static_cast<uint32_t>(layoutBarriers.size()),
		.pImageMemoryBarriers = layoutBarriers.data()
	};
	vkCmdPipelineBarrier2(cmd, &depInfo);

	// setup the attachments (color and depth) and begin rendering (dynamic)
	VkRenderingAttachmentInfo colorAttachInfo
	{
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
		.imageView = swapchainImageViews[imageIndex],
		.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR, // clear the image
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE, // keep data for presentation
		.clearValue{.color{0.01f, 0.01f, 0.01f, 1}}
	};
	VkRenderingAttachmentInfo depthAttachInfo
	{
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
		.imageView = depthImageView,
		.imageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR, // clear the depth data
		.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE, // don't care after rendering
		.clearValue{.depthStencil{1.0f, 0}}
	};
	VkRenderingInfo renderingInfo
	{
		.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
		.renderArea
		{
			.offset{0, 0},
			.extent{swapchainWidth, swapchainHeight}
		},
		.layerCount = 1,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorAttachInfo,
		.pDepthAttachment = &depthAttachInfo
	};

	// begin dynamic rendering
	vkCmdBeginRendering(cmd, &renderingInfo);
	{
		// set the viewpot and scissor state
		VkViewport viewport
		{
			.x = 0, .y = 0,
			.width = static_cast<float>(swapchainWidth),
			.height = static_cast<float>(swapchainHeight)
		};
		vkCmdSetViewport(cmd, 0, 1, &viewport);

		VkRect2D scissor
		{
			.offset{.x = 0, .y = 0 },
			.extent{.width = swapchainWidth, .height = swapchainHeight}
		};
		vkCmdSetScissor(cmd, 0, 1, &scissor);

		// draw our triangle
		vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline->GetPipeline());
		vkCmdDraw(cmd, 3, 1, 0, 0);
	}
	// end dynamic rendering
	vkCmdEndRendering(cmd);

	// transition the image from color attachment to presentation so we can show it
	VkImageMemoryBarrier2 presentLayoutBarrier
	{
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
		.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
		.srcAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
		.dstStageMask = VK_PIPELINE_STAGE_2_NONE, // nothing is waiting, but the cache is flushed and layout is transition
		.dstAccessMask = 0,
		.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		.image = swapchainImages[imageIndex],
		.subresourceRange
		{
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1,
		}
	};
	VkDependencyInfo presentDepInfo
	{
		.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
		.imageMemoryBarrierCount = 1,
		.pImageMemoryBarriers = &presentLayoutBarrier
	};
	vkCmdPipelineBarrier2(cmd, &presentDepInfo);

	vkEndCommandBuffer(cmd);

	// ensure swapchain image is actually vailable to start color output
	VkSemaphoreSubmitInfo imageAcquireWaitInfo
	{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
		.semaphore = imageAcquireSemaphore,
		.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT // wait before drawing to image
	};
	// signal that the image can be presented
	std::vector<VkSemaphoreSubmitInfo> semaphoreSignals
	{
		{ // render work completion signal
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
			.semaphore = renderCompleteSemaphores[imageIndex],
			.stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT
		},
		{ // entire frame is completed (timeline)
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
			.semaphore = timelineSemaphore,
			.value = signalValue,
			.stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT
		}
	};
	VkCommandBufferSubmitInfo cmdSubmitInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
		.commandBuffer = cmd,
	};
	VkSubmitInfo2 submitInfo
	{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
		.waitSemaphoreInfoCount = 1,
		.pWaitSemaphoreInfos = &imageAcquireWaitInfo, // ensure the image is ready
		.commandBufferInfoCount = 1,
		.pCommandBufferInfos = &cmdSubmitInfo,
		.signalSemaphoreInfoCount = static_cast<uint32_t>(semaphoreSignals.size()),
		.pSignalSemaphoreInfos = semaphoreSignals.data()
	};
	vkQueueSubmit2(gfxQueue, 1, &submitInfo, VK_NULL_HANDLE);

	// present the image
	VkPresentInfoKHR presentInfo{
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &renderCompleteSemaphores[imageIndex], // render work completed semaphore
		.swapchainCount = 1,
		.pSwapchains = &swapchain,
		.pImageIndices = &imageIndex,
		.pResults = nullptr
	};

	vkQueuePresentKHR(gfxQueue, &presentInfo);

	// TEMPORARY: ensure GPU finihed this frame before starting the next
	vkQueueWaitIdle(gfxQueue);
}

