//DB
#pragma once
#include <cstdint>
#include <functional>


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

static const Entity INVALID_ENTITY = { 0, 0 };

namespace std
{
	template<>
	struct hash<Entity>
	{
		size_t operator()(const Entity& e) const noexcept
		{
			// Combine index and generation into a single hash
			uint64_t combined = (uint64_t(e.index) << 32) | uint64_t(e.generation);
			return std::hash<uint64_t>()(combined);
		}
	};
}
