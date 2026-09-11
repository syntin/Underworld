//DB
#pragma once
#include "runtimeScene.h"

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

	void Update(World& world, float dt)
	{
		if (currentScene)
			currentScene->Update(world, dt);
	}




private:
	RuntimeScene* currentScene = nullptr;
};