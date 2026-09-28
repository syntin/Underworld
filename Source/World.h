//DB
#pragma once
#include "entityManager.h"
#include "componentManager.h"
#include "SceneGraph.h"
#include "worldSpawn.h"
#include "sceneManager.h"
#include "input.h"
#include "transformSystem.h"
#include "cameraSystem.h"

class CullingSystem;

class World
{
public:
	World();

public:
	void Initialize();
	void Update(float dt);
	void Shutdown();
	void UpdateTransforms();

public:
	EntityManager& GetEntityManager() { return entityManager; }
	ComponentManager& GetComponentManager() { return componentManager; }
	SceneGraph& GetSceneGraph() { return sceneGraph; }
	SceneManager& GetSceneManager() { return sceneManager; }
	EntitySpawner& GetSpawner() { return spawner; }
	Input& GetInput() { return input; }

private:
	EntityManager entityManager;
	ComponentManager componentManager;
	SceneGraph sceneGraph;
	SceneManager sceneManager;
	EntitySpawner spawner;
	Input input;

	CullingSystem* _cullingSystem = nullptr;
	TransformSystem* _transformSystem = nullptr;
	CameraSystem* _cameraSystem = nullptr;

};