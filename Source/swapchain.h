#pragma once

#include "device.h"

class SwapChain
{
public:
	SwapChain();
	virtual ~SwapChain();

public:
	void Initialize(Device device);
};

