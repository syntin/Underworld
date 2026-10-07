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
#include "assetRegistry.h"
#include "movementSystem.h"
#include "physicsBroadphaseSystem.h"
#include "physicsNarrowphaseSystem.h"
#include "rigidBodySolverSystem.h"

class CullingSystem;

class World
{
public:
	World();

public:
	void Initialize();
	void Update(float dt);
	void Shutdown();

public:
	EntityManager& GetEntityManager() { return entityManager; }
	ComponentManager& GetComponentManager() { return componentManager; }
	SceneGraph& GetSceneGraph() { return sceneGraph; }
	SceneManager& GetSceneManager() { return sceneManager; }
	EntitySpawner& GetSpawner() { return spawner; }
	Input& GetInput() { return input; }
	AssetRegistry& GetAssetRegistry() { return assetRegistry; }
	PhysicsBroadphaseSystem* GetBroadphaseSystem() { return _broadphaseSystem; }
	PhysicsNarrowphaseSystem* GetNarrowphaseSystem() { return _narrowphaseSystem; }
	RigidBodySolverSystem* GetRigidBodySolver() { return _rigidBodySolver; }

	float GetDeltaTime() const { return m_deltaTime; }
	void SetDeltaTime(float dt) { m_deltaTime = dt; }

private:
	EntityManager entityManager;
	ComponentManager componentManager;
	SceneGraph sceneGraph;
	SceneManager sceneManager;
	EntitySpawner spawner;
	Input input;
	AssetRegistry assetRegistry;
	MovementSystem movementSystem;

	CullingSystem* _cullingSystem = nullptr;
	TransformSystem* _transformSystem = nullptr;
	CameraSystem* _cameraSystem = nullptr;
	PhysicsBroadphaseSystem* _broadphaseSystem = nullptr;
	PhysicsNarrowphaseSystem* _narrowphaseSystem = nullptr;
	RigidBodySolverSystem* _rigidBodySolver = nullptr;
	
	float m_deltaTime = 0.0f;

};