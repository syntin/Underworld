#pragma once
#include <vector>
#include "utils.h"

class Extensions
{
public:
	Extensions();
	virtual ~Extensions();

public:
	void GetRequiredExtensions();

protected:
	void CheckExtensions();

private:
	std::vector<const char*> _extensions;
};

