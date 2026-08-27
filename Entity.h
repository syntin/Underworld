#pragma once
#include <cstdint>


struct Entity
{
	uint32_t index = 0;
	uint32_t generation = 0;

	bool IsValid() const { return generation != 0; }

	bool operator==(const Entity & other) const
	{
		return index == other.index && generation == other.generation;
	}

	bool operator!=(const Entity& other) const
	{
		return !(*this == other);
	}
};