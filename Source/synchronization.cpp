// Author: RC

#include <volk/volk.h>
#include "synchronization.h"
#include "utils.h"

Synchronization::Synchronization()
{

}

Synchronization::~Synchronization()
{

}

bool Synchronization::Initialize(Device device, Window window)
{
	VkSemaphoreTypeCreateInfo semaphoreTypeInfo
	{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
		.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE,
		.initialValue = MaxFramesInFlight
	};
	VkSemaphoreCreateInfo semaphoreInfo
	{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		.pNext = &semaphoreTypeInfo
	};
	if (vkCreateSemaphore(device.GetLogicalDevice(), &semaphoreInfo, nullptr, &_timelineSemaphore) != VK_SUCCESS)
	{
		showError("Unable to create the timeline semaphore", window.GetSDLWindow());
		return false;
	}
	return CreateSyncResources(device, window);
}

bool Synchronization::CreateSyncResources(Device device, Window window)
{
	// per-frame image-acquire semaphores
	for (FrameResources& res : _frameResources)
	{
		// create the binary semaphores
		VkSemaphoreCreateInfo semaphoreInfo{ .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
		if (vkCreateSemaphore(device.GetLogicalDevice(), &semaphoreInfo, nullptr, &res._imageAcquiredSemaphore) != VK_SUCCESS)
		{
			showError("Error creating the per-frame image-acquire semaphore", window.GetSDLWindow());
			return false;
		}
	}

	return true;
}

