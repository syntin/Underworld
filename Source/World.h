//DB
#pragma once
#include "entityManager.h"
#include "componentManager.h"
#include "SceneGraph.h"
#include "worldSpawn.h"
#include "sceneManager.h"

class World
{
public:
	World();

	void Initialize();
	void Update(float dt);
	void Shutdown();
	void UpdateTransforms();

	EntityManager& GetEntityManager() { return entityManager; }
	ComponentManager& GetComponentManager() { return componentManager; }
	SceneGraph& GetSceneGraph() { return sceneGraph; }
	SceneManager& GetSceneManager() { return sceneManager; }
	EntitySpawner& GetSpawner() { return spawner; }
private:
	EntityManager entityManager;
	ComponentManager componentManager;
	SceneGraph sceneGraph;
	SceneManager sceneManager;
	EntitySpawner spawner;
};