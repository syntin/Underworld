#pragma once
#include <vector>
#include <queue>
#include "Entity.h"

class EntityManager
{
public:
	EntityManager() = default;

	Entity CreateEntity()
	{
		uint32_t index;
		if (!m_freeIndices.empty())
		{
			index = m_freeIndices.front();
			m_freeIndices.pop();
			m_generations[index] + 1;
		}
		else
		{
			index = static_cast<uint32_t>(m_generations.size());
			m_generations.push_back(1); // generation starts at 1 for valid entities
		}

		Entity e;
		e.index = index;
		e.generation = m_generations[index];
		return e;
	}

	void DestroyEntity(Entity e)
	{
		if (!IsAlive(e)) return;

		m_generations[e.index] += 1; // invalidate old gen
		m_freeIndices.push(e.index);
	}

	bool IsAlive(Entity e) const
	{
		if (e.index >= m_generations.size()) return false;
		return m_generations[e.index] == e.generation && e.generation != 0;
	}

private:
	std::vector<uint32_t> m_generations;
	std::queue<uint32_t> m_freeIndices;

};