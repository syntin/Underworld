#include <iostream>
#include <chrono>
#include <thread>
#include "World.h"
#include "RuntimeScene.h"
#include "sceneRegistry.h"
#include "TestECS.h"
#include "cameraControllerSystem.h"


class TestScene : public RuntimeScene
{
public:
    void OnEnter(World& world) override
    {
        auto& spawner = world.GetSpawner();
        auto& components = world.GetComponentManager();

        root = spawner.SpawnEmpty();
        child1 = spawner.SpawnEmpty({ root });
        child2 = spawner.SpawnEmpty({ root });

        Transform* t1 = components.GetTransform(child1);
        Transform* t2 = components.GetTransform(child2);

        t1->position = { 1, 0, 0 };
        t2->position = { -1, 0, 0 };

        // --- Camera entity ---
        cameraEntity = spawner.SpawnEmpty();
        Camera cam;
        components.AddCamera(cameraEntity, cam);

        Transform* ct = components.GetTransform(cameraEntity);
        ct->position = { 0, 2, -5 };
        ct->rotation = { -10, 0, 0 };
    }

    void OnExit(World& world) override
    {

    }

    void Update(World& world, float dt) override
    {
        auto& components = world.GetComponentManager();

        Transform* t1 = components.GetTransform(child1);
        Transform* t2 = components.GetTransform(child2);

        t1->position.x += dt * 2.0f;   // move right
        t2->position.x -= dt * 2.0f;   // move left

        Transform* ct = components.GetTransform(cameraEntity);
        Camera* cam = components.GetCamera(cameraEntity);

        ct->rotation.y += dt * 10.0f;  
        ct->dirty = true;
        cam->dirty = true;

        
        cameraController.Update(world, dt);

        world.UpdateTransforms();

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