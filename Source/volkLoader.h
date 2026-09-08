// Author: RC

#pragma once

#include "instance.h"

class VolkLoader
{
public:
	VolkLoader();
	virtual ~VolkLoader();

public:
	bool Initialize(VulkanInstance vulkanInstance, Extensions& extensions);
	//VulkanInstance& GetVulkanInstance() { return _vulkanInstance; }
};




