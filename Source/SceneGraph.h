#pragma once
#include <vector>
#include <unordered_map>
#include "Entity.h"

// This is kinda minimal but will work, we shouldn't need to upgrade it
// until we need stuff like depth ordering

class SceneGraph
{
public:
	void SetParent(Entity child, Entity parent)
	{
		RemoveParent(child);

		m_parent[child.index] = parent;
		m_children[parent.index].push_back(child);

		// Mark dirty
		m_dirty[child.index] = true;
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

		// Mark dirty
		m_dirty[child.index] = true;
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

	bool IsDirty(Entity e) const
	{
		auto it = m_dirty.find(e.index);
		if (it == m_dirty.end())
			return false;
		return it->second;
	}

	void ClearDirty(Entity e)
	{
		m_dirty[e.index] = false;
	}

private:
	std::unordered_map<uint32_t, Entity> m_parent;
	std::unordered_map<uint32_t, std::vector<Entity>> m_children;
	std::unordered_map<uint32_t, bool > m_dirty;
};