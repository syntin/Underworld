#include <iostream>
#include <chrono>
#include <thread>
#include "World.h"
#include "RuntimeScene.h"
#include "SceneRegistry.h"
#include "TestECS.h"
#include "cameraControllerSystem.h"
#include "cameraControllerComponent.h"

class TestScene : public RuntimeScene
{
public:
    void OnEnter(World& world) override
    {
        auto& spawner = world.GetSpawner();
        auto& components = world.GetComponentManager();

        // Moving entities
        root = spawner.SpawnEmpty();
        child1 = spawner.SpawnEmpty({ root });
        child2 = spawner.SpawnEmpty({ root });

        Transform* t1 = components.GetTransform(child1);
        Transform* t2 = components.GetTransform(child2);

        t1->position = { 1, 0, 0 };
        t2->position = { -1, 0, 0 };

        // Camera entity 
        cameraEntity = spawner.SpawnEmpty();

        Camera cam;
        components.AddCamera(cameraEntity, cam);

        // Add WASD controller ONLY to this camera
        CameraControllerComponent ctrl;
        ctrl.moveSpeed = 5.0f;
        ctrl.rotateSpeed = 90.0f;
        components.AddCameraController(cameraEntity, ctrl);

        Transform* ct = components.GetTransform(cameraEntity);
        ct->position = { 0, 2, -5 };
        ct->rotation = { -10, 0, 0 };
    }

    void OnExit(World& world) override
    {
        // Nothing needed yet
    }

    void Update(World& world, float dt) override
    {
        auto& components = world.GetComponentManager();

        // Move entities 
        Transform* t1 = components.GetTransform(child1);
        Transform* t2 = components.GetTransform(child2);

        t1->position.x += dt * 2.0f;
        t2->position.x -= dt * 2.0f;

        // Camera WASD movement 
        cameraController.Update(world, dt);

        // Update world transforms
        world.UpdateTransforms();

        // Debug print
        auto& transforms = components.GetAllTransforms();
        auto& entities = components.GetTransformEntities();

        std::cout << "---- Frame ----\n";
        for (size_t i = 0; i < transforms.size(); ++i)
        {
            const Entity& e = entities[i];
            const Transform& t = transforms[i];

            std::cout << "Entity " << e.index
                << " local=(" << t.position.x << ", "
                << t.position.y << ", "
                << t.position.z << ") "
                << " world=(" << t.worldPosition.x << ", "
                << t.worldPosition.y << ", "
                << t.worldPosition.z << ")\n";
        }
    }

private:
    Entity root;
    Entity child1;
    Entity child2;

    Entity cameraEntity;
    CameraControllerSystem cameraController;
};

int TestECS()
{
    World world;
    world.Initialize();

    SceneRegistry::Register("TestScene", []() { return new TestScene(); });

    world.GetSceneManager().LoadScene("TestScene", world);

    for (int i = 0; i < 200; ++i)
    {
        world.GetSceneManager().Update(world, 0.016f);
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    world.Shutdown();
    return 0;
}