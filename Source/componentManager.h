#pragma once
#include "Entity.h"
#include "ComponentPool.h"
#include "Transform.h"
#include "camera.h"

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

	// Camera

	Camera* AddCamera(Entity e, const Camera& c)
	{
		return m_cameraPool.AddComponent(e, c);
	}

	Camera* GetCamera(Entity e)
	{
		return m_cameraPool.GetComponent(e);
	}

	const Camera* GetCamera(Entity e) const
	{
		return m_cameraPool.GetComponent(e);
	}

	bool RemoveCamera(Entity e)
	{
		return m_cameraPool.RemoveComponent(e);
	}

	bool HasCamera(Entity e) const
	{
		return m_cameraPool.HasComponent(e);
	}

	std::vector<Camera>& GetAllCameras()
	{
		return m_cameraPool.GetComponents();
	}
	
	std::vector<Entity>& GetCameraEntities()
	{
		return m_cameraPool.GetEntities();
	}
	// TO DO: Add more components

private:
	ComponentPool<Transform> m_transformPool;
	ComponentPool<Camera> m_cameraPool;
};