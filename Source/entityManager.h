//DB
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
			m_generations[index] += 1;
		}
		else
		{
			index = static_cast<uint32_t>(m_generations.size());
			m_generations.push_back(1); // generation starts at 1 for valid entities
		}

		Entity e;
		e.index = index;
		e.generation = m_generations[index];

		m_activeEntities.push_back(e);
		return e;
	}

	void DestroyEntity(Entity e)
	{
		if (!IsAlive(e))
			return;

		m_generations[e.index] += 1; // invalidate old gen
		m_freeIndices.push(e.index);

		for (size_t i = 0; i < m_activeEntities.size(); ++i)
		{
			if (m_activeEntities[i] == e)
			{
				m_activeEntities.erase(m_activeEntities.begin() + i);
				break;
			}
		}
	}

	bool IsAlive(Entity e) const
	{
		if (e.index >= m_generations.size()) return false;
		return m_generations[e.index] == e.generation && e.generation != 0;
	}

	const std::vector<Entity>& GetAllEntities() const
	{
		return m_activeEntities;
	}

private:
	std::vector<uint32_t> m_generations;
	std::queue<uint32_t> m_freeIndices;
	std::vector<Entity> m_activeEntities;
};