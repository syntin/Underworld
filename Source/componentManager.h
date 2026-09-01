#pragma once
#include "Entity.h"
#include "ComponentPool.h"
#include "Transform.h"
#include "camera.h"
#include "mesh.h"
#include "material.h"

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

	// Mesh
	Mesh* AddMesh(Entity e, const Mesh& m)
	{
		return m_meshPool.AddComponent(e, m);
	}

	Mesh* GetMesh(Entity e)
	{
		return m_meshPool.GetComponent(e);
	}

	const Mesh* GetMesh(Entity e) const
	{
		return m_meshPool.GetComponent(e);
	}

	bool RemoveMesh(Entity e)
	{
		return m_meshPool.RemoveComponent(e);
	}

	bool HasMesh(Entity e) const
	{
		return m_meshPool.HasComponent(e);
	}

	std::vector<Mesh>& GetAllMeshes()
	{
		return m_meshPool.GetComponents();
	}

	std::vector<Entity>& GetMeshEntities()
	{
		return m_meshPool.GetEntities();
	}

	// Material
	Material* AddMaterial(Entity e, const Material& m)
	{
		return m_materialPool.AddComponent(e, m);
	}

	Material* GetMaterial(Entity e)
	{
		return m_materialPool.GetComponent(e);
	}

	const Material* GetMaterial(Entity e) const
	{
		return m_materialPool.GetComponent(e);
	}

	bool RemoveMaterial(Entity e)
	{
		return m_materialPool.RemoveComponent(e);
	}

	bool HasMaterial(Entity e) const
	{
		return m_materialPool.HasComponent(e);
	}

	std::vector<Material>& GetAllMaterials()
	{
		return m_materialPool.GetComponents();
	}

	std::vector<Entity>& GetMaterialEntities()
	{
		return m_materialPool.GetEntities();
	}

	// TO DO: Add components as needed

private:
	ComponentPool<Transform> m_transformPool;
	ComponentPool<Camera> m_cameraPool;
	ComponentPool<Mesh> m_meshPool;
	ComponentPool<Material> m_materialPool;
};