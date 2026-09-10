#pragma once

#include <Volk/volk.h>
#include <vector>

class Semaphore
{
public:
	Semaphore();
	virtual ~Semaphore();

public:
	std::vector<VkSemaphore>& GetRenderCompleted() { return _renderCompleted; }
	void ClearRenderCompleted() { _renderCompleted.clear(); }

private:
	std::vector<VkSemaphore> _renderCompleted;
};