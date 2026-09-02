#include "vulkanWrapper.h"
#include <Windows.h>
#include "utils.h"


VulkanWrapper::VulkanWrapper()
{

}

VulkanWrapper::~VulkanWrapper()
{
    Destroy();
}

void VulkanWrapper::InitializeVulkan(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    chk(SDL_Init(SDL_INIT_VIDEO));
    chk(SDL_Vulkan_LoadLibrary(NULL));

    _vulkanInstance.Initialize();
    _device.Initialize(_vulkanInstance.GetInstance(), _device);
    _queue.Initialize(_vulkanInstance.GetInstance(), _device);
	_debugMessenger.Initialize();
	_extensions.Initialize();
	_window.CreateSDLwindow(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
    _swapChain.Initialize();
    _depthAttachment.Initialize();
    _mesh.Initialize();
    _vertexData.Initialize();
    _shaderData.Initialize();
    _synchronization.Initialize();
    _commandPool.Initialize();

    _textureImages.Initialize();
    _descriptor.Initialize();
    _slangShader.Initialize();
    _loadShader.Initialize();
    _pipeline.Initialize();

    RunRenderLoop();
    Destroy();
}

void VulkanWrapper::RunRenderLoop()
{
	/*
	uint64_t lastTime{ SDL_GetTicks() };
	bool quit{ false };
	while (!quit) {
		// Sync
		chk(vkWaitForFences(device, 1, &fences[frameIndex], true, UINT64_MAX));
		chk(vkResetFences(device, 1, &fences[frameIndex]));
		chkSwapchain(vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, imageAcquiredSemaphores[frameIndex], VK_NULL_HANDLE, &imageIndex));
		// Update shader data
		shaderData.projection = glm::perspective(glm::radians(45.0f), (float)windowSize.x / (float)windowSize.y, 0.1f, 32.0f);
		shaderData.view = glm::translate(glm::mat4(1.0f), camPos);
		for (auto i = 0; i < 3; i++) {
			auto instancePos = glm::vec3((float)(i - 1) * 3.0f, 0.0f, 0.0f);
			shaderData.model[i] = glm::translate(glm::mat4(1.0f), instancePos) * glm::mat4_cast(glm::quat(objectRotations[i]));
		}
		memcpy(shaderDataBuffers[frameIndex].allocationInfo.pMappedData, &shaderData, sizeof(ShaderData));
		// Build command buffer
		auto cb = commandBuffers[frameIndex];
		chk(vkResetCommandBuffer(cb, 0));
		VkCommandBufferBeginInfo cbBI{ .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT };
		chk(vkBeginCommandBuffer(cb, &cbBI));
		std::array<VkImageMemoryBarrier2, 2> outputBarriers{
			VkImageMemoryBarrier2{
				.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
				.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
				.srcAccessMask = 0,
				.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
				.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
				.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.newLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
				.image = outputBarriers[imageIndex],
				.subresourceRange{.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .levelCount = 1, .layerCount = 1 }
			},
			VkImageMemoryBarrier2{
				.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
				.srcStageMask = VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT,
				.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
				.dstStageMask = VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT,
				.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
				.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.newLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
				.image = outputBarriers[1],
				.subresourceRange{.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, .levelCount = 1, .layerCount = 1 }
			}
		};
		VkDependencyInfo barrierDependencyInfo{
			.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
			.imageMemoryBarrierCount = 2,
			.pImageMemoryBarriers = outputBarriers.data()
		};
		vkCmdPipelineBarrier2(cb, &barrierDependencyInfo);
		VkRenderingAttachmentInfo colorAttachmentInfo{
			.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
			.imageView = swapchainImageViews[imageIndex],
			.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
			.clearValue{.color{ 0.0f, 0.0f, 0.0f, 1.0f }}
		};
		VkImageView depthImageView;
		VkRenderingAttachmentInfo depthAttachmentInfo;
		depthAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
		depthAttachmentInfo.imageView = depthImageView;
		depthAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
		depthAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachmentInfo.clearValue = { .depthStencil = {1.0f,  0} };

		VkRenderingInfo renderingInfo{
			.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
			.renderArea{.extent{.width = static_cast<uint32_t>(windowSize.x), .height = static_cast<uint32_t>(windowSize.y) }},
			.layerCount = 1,
			.colorAttachmentCount = 1,
			.pColorAttachments = &colorAttachmentInfo,
			.pDepthAttachment = &depthAttachmentInfo
		};
		vkCmdBeginRendering(cb, &renderingInfo);
		VkViewport vp{ .width = static_cast<float>(windowSize.x), .height = static_cast<float>(windowSize.y), .minDepth = 0.0f, .maxDepth = 1.0f };
		vkCmdSetViewport(cb, 0, 1, &vp);
		VkRect2D scissor{ .extent{.width = static_cast<uint32_t>(windowSize.x), .height = static_cast<uint32_t>(windowSize.y) } };
		vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
		vkCmdSetScissor(cb, 0, 1, &scissor);
		vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSetTex, 0, nullptr);
		VkDeviceSize vOffset{ 0 };
		vkCmdBindVertexBuffers(cb, 0, 1, &vBuffer, &vOffset);
		vkCmdBindIndexBuffer(cb, vBuffer, vBufSize, VK_INDEX_TYPE_UINT16);
		vkCmdPushConstants(cb, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(VkDeviceAddress), &shaderDataBuffers[frameIndex].deviceAddress);
		vkCmdDrawIndexed(cb, indexCount, 3, 0, 0, 0);
		vkCmdEndRendering(cb);
		VkImageMemoryBarrier2 barrierPresent{
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
			.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
			.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
			.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
			.dstAccessMask = 0,
			.oldLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
			.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			.image = swapchainImages[imageIndex],
			.subresourceRange{.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .levelCount = 1, .layerCount = 1 }
		};
		VkDependencyInfo barrierPresentDependencyInfo{ .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO, .imageMemoryBarrierCount = 1, .pImageMemoryBarriers = &barrierPresent };
		vkCmdPipelineBarrier2(cb, &barrierPresentDependencyInfo);
		chk(vkEndCommandBuffer(cb));
		// Submit to graphics queue
		VkSemaphoreSubmitInfo waitSemaphoreInfo{ .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO, .semaphore = imageAcquiredSemaphores[frameIndex], .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkCommandBufferSubmitInfo commandBufferSubmitInfo{ .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO, .commandBuffer = cb };
		VkSemaphoreSubmitInfo signalSemaphoreInfo{ .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO, .semaphore = renderCompleteSemaphores[imageIndex], .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSubmitInfo2 submitInfo{
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
			.waitSemaphoreInfoCount = 1,
			.pWaitSemaphoreInfos = &waitSemaphoreInfo,
			.commandBufferInfoCount = 1,
			.pCommandBufferInfos = &commandBufferSubmitInfo,
			.signalSemaphoreInfoCount = 1,
			.pSignalSemaphoreInfos = &signalSemaphoreInfo,
		};
		chk(vkQueueSubmit2(queue, 1, &submitInfo, fences[frameIndex]));
		frameIndex = (frameIndex + 1) % maxFramesInFlight;
		VkPresentInfoKHR presentInfo{
			.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &renderCompleteSemaphores[imageIndex],
			.swapchainCount = 1,
			.pSwapchains = &swapchain,
			.pImageIndices = &imageIndex
		};
		chkSwapchain(vkQueuePresentKHR(queue, &presentInfo));
		// Event polling
		float elapsedTime{ (SDL_GetTicks() - lastTime) / 1000.0f };
		lastTime = SDL_GetTicks();
		for (SDL_Event event; SDL_PollEvent(&event);) {
			if (event.type == SDL_EVENT_QUIT) {
				quit = true;
				break;
			}
			if (event.type == SDL_EVENT_MOUSE_MOTION) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					objectRotations[shaderData.selected].x -= (float)event.motion.yrel * elapsedTime;
					objectRotations[shaderData.selected].y += (float)event.motion.xrel * elapsedTime;
				}
			}
			if (event.type == SDL_EVENT_MOUSE_WHEEL) {
				camPos.z += (float)event.wheel.y * elapsedTime * 10.0f;
			}
			if (event.type == SDL_EVENT_KEY_DOWN) {
				if (event.key.key == SDLK_PLUS || event.key.key == SDLK_KP_PLUS) {
					shaderData.selected = (shaderData.selected < 2) ? shaderData.selected + 1 : 0;
				}
				if (event.key.key == SDLK_MINUS || event.key.key == SDLK_KP_MINUS) {
					shaderData.selected = (shaderData.selected > 0) ? shaderData.selected - 1 : 2;
				}
			}
			// Window resize
			if (event.type == SDL_EVENT_WINDOW_RESIZED) {
				updateSwapchain = true;
			}
		}
		if (updateSwapchain) {
			chk(SDL_GetWindowSize(window, &windowSize.x, &windowSize.y));
			updateSwapchain = false;
			chk(vkDeviceWaitIdle(device));
			chk(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(devices[deviceIndex], surface, &surfaceCaps));
			swapchainCI.oldSwapchain = swapchain;
			swapchainCI.imageExtent = { .width = static_cast<uint32_t>(windowSize.x), .height = static_cast<uint32_t>(windowSize.y) };
			chk(vkCreateSwapchainKHR(device, &swapchainCI, nullptr, &swapchain));
			for (auto i = 0; i < imageCount; i++) {
				vkDestroyImageView(device, swapchainImageViews[i], nullptr);
			}
			chk(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr));
			swapchainImages.resize(imageCount);
			chk(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages.data()));
			swapchainImageViews.resize(imageCount);
			for (auto i = 0; i < imageCount; i++) {
				VkImageViewCreateInfo viewCI{ .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, .image = swapchainImages[i], .viewType = VK_IMAGE_VIEW_TYPE_2D, .format = imageFormat, .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .levelCount = 1, .layerCount = 1} };
				chk(vkCreateImageView(device, &viewCI, nullptr, &swapchainImageViews[i]));
			}
			for (auto& semaphore : renderCompleteSemaphores) {
				vkDestroySemaphore(device, semaphore, nullptr);
			}
			renderCompleteSemaphores.resize(imageCount);
			for (auto& semaphore : renderCompleteSemaphores) {
				chk(vkCreateSemaphore(device, &semaphoreCI, nullptr, &semaphore));
			}
			vkDestroySwapchainKHR(device, swapchainCI.oldSwapchain, nullptr);
			vmaDestroyImage(allocator, depthImage, depthImageAllocation);
			vkDestroyImageView(device, depthImageView, nullptr);
			depthImageCI.extent = { .width = static_cast<uint32_t>(windowSize.x), .height = static_cast<uint32_t>(windowSize.y), .depth = 1 };
			VmaAllocationCreateInfo allocCI{ .flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT, .usage = VMA_MEMORY_USAGE_AUTO };
			chk(vmaCreateImage(allocator, &depthImageCI, &allocCI, &depthImage, &depthImageAllocation, nullptr));
			VkImageViewCreateInfo viewCI{ .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, .image = depthImage, .viewType = VK_IMAGE_VIEW_TYPE_2D, .format = depthFormat, .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT, .levelCount = 1, .layerCount = 1 } };
			chk(vkCreateImageView(device, &viewCI, nullptr, &depthImageView));
		}
	}*/
}

void VulkanWrapper::Destroy()
{
    HWND hWnd = _window.GetHWnd();
    DestroyWindow(hWnd);
}