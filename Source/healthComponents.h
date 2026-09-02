#pragma once
#include <cstdint>

struct Health
{
	int32_t current = 100;
	int32_t max = 100;

	bool dirty = true;
};


class HealthComponents
{
public:
	HealthComponents();
	void ~HealthComponents();
};

