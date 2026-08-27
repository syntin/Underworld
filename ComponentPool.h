#pragma once
#include <vector>
#include <unordered_map>
#include "Entity.h"

template<typename T>
class ComponentPool
{
public:
	T* AddComponent(Entity e, const T& data)
	{
		if (HasComponent(e))
		{
			//overwrite existing
			T& existing = m_components[m_entityToIndex[e.index]];
			existing = data;
			return &existing;
		}

		uint32_t index = static_cast<uint32_t>(m_components.size());
		m_components.push_back(data);
		m_entities.push_back(e);
		m_entityToIndex[e.index] = index;
		return &m_components[index];
	}

	bool RemoveComponent(Entity e)
	{
		auto it = m_entityToIndex.find(e.index);
		if (it == m_entityToIndex.end())
			return false;

		uint32_t index = it->second;
		uint32_t lastIndex = static_cast<uint32_t>(m_components.size() - 1);

		// swap with last
		m_components[index] = m_components[lastIndex];
		m_entities[index] = m_entities[lastIndex];

		// update moved entity's idnex
		m_entityToIndex[m_entities[index].index] = index; //buffalo buffalo buffalo buffalo

		// remove last
		m_components.pop_back();
		m_entities.pop_back();
		m_entityToIndex.erase(it);

		return true;
	}

	T* GetComponent(Entity e)
	{
		auto it = m_entityToIndex.find(e.index);
		if (it == m_entityToIndex.end())
			return nullptr;

		return &m_components[it->second];
	}

	const T* GetComponent(Entity e) const
	{
		auto it = m_entityToIndex.find(e.index);
		if (it == m_entityToIndex.end())
			return nullptr;

		return &m_components[it->second];
	}

	bool HasComponent(Entity e) const
	{
		return m_entityToIndex.find(e.index) != m_entityToIndex.end();
	}

	std::vector<T>& GetComponents() { return m_components; }
	const std::vector<T>& GetComponents() const { return m_components; }

	std::vector<Entity>& GetEntities() { return m_entities; }
	const std::vector<Entity>& GetEntities() const { return m_entities; }

private:
	std::vector<T> m_components;
	std::vector<Entity> m_entities;
	std::unordered_map<uint32_t, uint32_t> m_entityToIndex;
};