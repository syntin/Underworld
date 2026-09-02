#pragma once
#include <vector>
#include "utils.h"

class Extensions
{
public:
	Extensions();
	virtual ~Extensions();

public:
	void Initialize();
	uint32_t GetExtensionsCount() { return _extensionsCount; }
	char const* const* Data() { return _extensions;  }

protected:
	void GetRequiredExtensions();
	void CheckExtensions();

private:
	std::vector<const char*> _extensionVector;
	uint32_t _extensionsCount = 0;
	char const* const* _extensions = nullptr;
};

