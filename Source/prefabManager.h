//DB
#pragma once
#include <unordered_map>
#include "prefab.h"
#include "scene.h"
#include "componentManager.h"
#include "entityManager.h"
#include "prefabSerializer.h"

class PrefabManager
{
public:
	PrefabManager(Scene& scene, ComponentManager& components, EntityManager& entities);

	// Create a prefab from an existing entity hierarchy
	int CreatePrefab(Entity root);

	// Instantiate a prefab into the scene
	Entity Instantiate(int prefabID);

	void Save(int prefabID, const std::string& path);
	int Load(const std::string& path);

private:
	Scene& m_scene;
	ComponentManager& m_components;
	EntityManager& m_entities;

	std::unordered_map<int, Prefab> m_prefabs;
	int m_nextPrefabID = 1;

	void CopyEntityComponents(const PrefabEntity& pe, Entity dst);
};