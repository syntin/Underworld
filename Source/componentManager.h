#pragma once
#include "Entity.h"
#include "componentPool.h"
#include "Transform.h"
#include "camera.h"
#include "mesh.h"
#include "material.h"
#include "velocity.h"
#include "healthComponents.h"
#include "colliderComponents.h"
#include "lightComponents.h"
#include "scriptComponent.h"
#include "audio.h"

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
	ECS::Mesh* AddMesh(Entity e, const ECS::Mesh& m)
	{
		return m_meshPool.AddComponent(e, m);
	}

	ECS::Mesh* GetMesh(Entity e)
	{
		return m_meshPool.GetComponent(e);
	}

	const ECS::Mesh* GetMesh(Entity e) const
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

	std::vector<ECS::Mesh>& GetAllMeshes()
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

	// Velocity
	Velocity* AddVelocity(Entity e, const Velocity& v)
	{
		return m_velocityPool.AddComponent(e, v);
	}

	Velocity* GetVelocity(Entity e)
	{
		return m_velocityPool.GetComponent(e);
	}

	bool HasVelocity(Entity e) const
	{
		return m_velocityPool.HasComponent(e);
	}

	bool RemoveVelocity(Entity e)
	{
		return m_velocityPool.RemoveComponent(e);
	}

	std::vector<Velocity>& GetAllVelocities()
	{
		return m_velocityPool.GetComponents();
	}

	std::vector<Entity>& GetVelocityEntities()
	{
		return m_velocityPool.GetEntities();
	}

	// Health
	Health* AddHealth(Entity e, const Health& h)
	{
		return m_healthPool.AddComponent(e, h);
	}

	Health* GetHealth(Entity e)
	{
		return m_healthPool.GetComponent(e);
	}

	bool HasHealth(Entity e) const
	{
		return m_healthPool.HasComponent(e);
	}

	bool RemoveHealth(Entity e)
	{
		return m_healthPool.RemoveComponent(e);
	}

	std::vector<Health>& GetAllHealth()
	{
		return m_healthPool.GetComponents();
	}

	std::vector<Entity>& GetHealthEntities()
	{
		return m_healthPool.GetEntities();
	}

	// Collider
	Collider* AddCollider(Entity e, const Collider& c)
	{
		return m_colliderPool.AddComponent(e, c);
	}

	Collider* GetCollider(Entity e)
	{
		return m_colliderPool.GetComponent(e);
	}

	bool HasCollider(Entity e) const
	{
		return m_colliderPool.HasComponent(e);
	}

	bool RemoveCollider(Entity e)
	{
		return m_colliderPool.RemoveComponent(e);
	}

	std::vector<Entity>& GetColliderEntities()
	{
		return m_colliderPool.GetEntities();
	}

	// Light
	Light* AddLight(Entity e, const Light& l)
	{
		return m_lightPool.AddComponent(e, l);
	}

	Light* GetLight(Entity e)
	{
		return m_lightPool.GetComponent(e);
	}

	bool HasLight(Entity e) const
	{
		return m_lightPool.HasComponent(e);
	}

	bool RemoveLight(Entity e)
	{
		return m_lightPool.RemoveComponent(e);
	}

	std::vector<Entity>& GetLightEntities()
	{
		return m_lightPool.GetEntities();
	}

	// Script
	Script* AddScript(Entity e, const Script& s)
	{
		return m_scriptPool.AddComponent(e, s);
	}

	Script* GetScript(Entity e)
	{
		return m_scriptPool.GetComponent(e);
	}

	bool HasScript(Entity e) const
	{
		return m_scriptPool.HasComponent(e);
	}

	bool RemoveScript(Entity e)
	{
		return m_scriptPool.RemoveComponent(e);
	}

	std::vector<Entity>& GetScriptEntities()
	{
		return m_scriptPool.GetEntities();
	}

	//Audio
	Audio* AddAudio(Entity e, const Audio& a)
	{
		return m_audioPool.AddComponent(e, a);
	}

	Audio* GetAudio(Entity e)
	{
		return m_audioPool.GetComponent(e);
	}

	bool HasAudio(Entity e) const
	{
		return m_audioPool.HasComponent(e);
	}

	bool RemoveAudio(Entity e)
	{
		return m_audioPool.RemoveComponent(e);
	}

	std::vector<Entity>& GetAudioEntities()
	{
		return m_audioPool.GetEntities();
	}

	// TO DO: Add components as needed

private:
	ComponentPool<Transform> m_transformPool;
	ComponentPool<Camera> m_cameraPool;
	ComponentPool<ECS::Mesh> m_meshPool;
	ComponentPool<Material> m_materialPool;
	ComponentPool<Velocity> m_velocityPool;
	ComponentPool<Health> m_healthPool;
	ComponentPool<Collider> m_colliderPool;
	ComponentPool<Light> m_lightPool;
	ComponentPool<Script> m_scriptPool;
	ComponentPool<Audio> m_audioPool;
};