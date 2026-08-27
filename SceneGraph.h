#pragma once
#include <vector>
#include <unordered_map>
#include "Entity.h"

// This is kinda minimal but will work, we shouldn't need to upgrade it
// until we need stuff like dirty flags and depth ordering

class SceneGraph
{
public:
	void SetParent(Entity child, Entity parent)
	{
		RemoveParent(child);

		m_parent[child.index] = parent;
		m_children[parent.index].push_back(child);
	}

	void RemoveParent(Entity child)
	{
		auto it = m_parent.find(child.index);
		if (it == m_parent.end())
			return;

		Entity parent = it->second;
		m_parent.erase(it);

		auto& siblings = m_children[parent.index];
		for (size_t i = 0; i < siblings.size(); ++i)
		{
			if (siblings[i].index == child.index &&
				siblings[i].generation == child.generation)
			{
				siblings.erase(siblings.begin() + i);
				break;
			}
		}
	}

	Entity GetParent(Entity child) const
	{
		auto it = m_parent.find(child.index);
		if (it == m_parent.end())
		{
			return Entity{};
		}
		return it->second;
	}

	const std::vector<Entity>& GetChildren(Entity parent) const
	{
		static std::vector<Entity> empty;
		auto it = m_children.find(parent.index);
		if (it == m_children.end())
			return empty;
		return it->second;
	}


private:
	std::unordered_map<uint32_t, Entity> m_parent;
	std::unordered_map<uint32_t, std::vector<Entity>> m_children;
};