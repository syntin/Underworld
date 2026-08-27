#pragma once
#include "Entity.h"
#include "ComponentPool.h"
#include "Transform.h"

class ComponentManager
{
public:
	//Transform
	Transform* AddTransform(Entity e, const Transform& t)
	{
		return m_transformPool.AddComponent(e, t);
	}

	Transform* GetTransform(Entity e)
	{
		return m_transformPool.GetComponent(e);
	}

	const Transform* GetTransform(Entity e) const
	{
		return m_transformPool.GetComponent(e);
	}

	bool RemoveTransform(Entity e)
	{
		return m_transformPool.RemoveComponent(e);
	}

	bool HasTransform(Entity e) const
	{
		return m_transformPool.HasComponent(e);
	}

	std::vector<Transform>& GetAllTransforms()
	{
		return m_transformPool.GetComponents();
	}

	std::vector<Entity>& GetTransformEntities()
	{
		return m_transformPool.GetEntities();
	}

	// TO DO: Add other components like Camera, Mesh and all the other stuff

private:
	ComponentPool<Transform> m_transformPool;

};