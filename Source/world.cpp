//DB
#include "World.h"
#include <iostream>
#include "cullingSystem.h"
#include "transformSystem.h"
#include "cameraSystem.h"

World::World()
    : spawner(*this)
{
}

void World::Initialize()
{
	std::cout << "World initialized.\n";
    _transformSystem = new TransformSystem();
    _cameraSystem = new CameraSystem();
    _cullingSystem = new CullingSystem();

}

void World::Update(float dt)
{
    m_deltaTime = dt;

    input.Update();
    
    // Movement system (velocity, acceleration, damping)
    movementSystem.Update(*this);

    // Update transforms (worldMatrix, hierarchy)
    _transformSystem->Update(componentManager, sceneGraph);

    // Update camera matrices (view + projection)
    _cameraSystem->Update(componentManager);

    // Run frustum culling (uses worldMatrix + camera matrices)
    _cullingSystem->Update(*this);

	//add shit here later
}

void World::Shutdown()
{
	std::cout << "world shutdown.\n";

    delete _transformSystem;
    delete _cameraSystem;
    delete _cullingSystem;
}

