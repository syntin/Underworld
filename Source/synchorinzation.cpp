#include "synchronization.h"

Synchronization::Synchronization()
{

}

Synchronization::!Synchronization()
{

}

void Synchronization::Initialize()
{
	VkSemaphoreCreateInfo semaphoreCI{ .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
	VkFenceCreateInfo fenceCI{ .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, .flags = VK_FENCE_CREATE_SIGNALED_BIT };
	for (auto i = 0; i < maxFramesInFlight; i++) {
		chk(vkCreateFence(device, &fenceCI, nullptr, &fences[i]));
		chk(vkCreateSemaphore(device, &semaphoreCI, nullptr, &imageAcquiredSemaphores[i]));
	}
	renderCompleteSemaphores.resize(swapchainImages.size());
	for (auto& semaphore : renderCompleteSemaphores) {
		chk(vkCreateSemaphore(device, &semaphoreCI, nullptr, &semaphore));
	}
}