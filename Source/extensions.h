// Author: RC

#pragma once
#include <vector>
#include "utils.h"

// Author: RC

class Extensions
{
public:
	Extensions();
	virtual ~Extensions();

public:
	void Initialize();
	uint32_t GetExtensionsCount() { return _extensionCount; }
	char const* const* Data() { return _extensions;  }

private:
	std::vector<const char*> _extensionVector;
	uint32_t _extensionCount = 0;
	char const* const* _extensions = nullptr;

public:

};


