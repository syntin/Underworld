//DB
#pragma once
#include "runtimeScene.h"
#include "sceneRegistry.h"

class SceneManager
{
public:
	void SetScene(RuntimeScene* newScene, World& world)
	{
		if (currentScene)
			currentScene->OnExit(world);

		currentScene = newScene;

		if (currentScene)
			currentScene->OnEnter(world);
	}

	void LoadScene(const std::string& name, World& world)
	{
		RuntimeScene* newScene = SceneRegistry::Create(name);
		SetScene(newScene, world);
	}

	void Update(World& world, float dt)
	{
		if (currentScene)
			currentScene->Update(world, dt);
	}

private:
	RuntimeScene* currentScene = nullptr;
};