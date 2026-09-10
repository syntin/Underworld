#include <iostream>
#include <chrono>
#include <thread>
#include "World.h"
#include "TestECS.h"

int TestECS()
{
    World world;
    world.Initialize();

    auto& spawner = world.GetSpawner();
    auto& components = world.GetComponentManager();
    auto& sceneGraph = world.GetSceneGraph();

    // Spawn root
    Entity root = spawner.SpawnEmpty();

    // Spawn two children under root
    Entity child1 = spawner.SpawnEmpty({ root });
    Entity child2 = spawner.SpawnEmpty({ root });

    // Initial local positions
    Transform* t1 = components.GetTransform(child1);
    Transform* t2 = components.GetTransform(child2);

    t1->position = { 1, 0, 0 };   
    t2->position = { -1, 0, 0 };  

    for (int frame = 0; frame < 10; ++frame)
    {
        // Move in opposite directions
        t1->position.x += 0.5f;   // moves right
        t2->position.x -= 0.5f;   // moves left

        // Update world transforms
        world.UpdateTransforms();

        auto& transforms = components.GetAllTransforms();
        auto& entities = components.GetTransformEntities();

        std::cout << "---- Frame " << frame << " ----\n";

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

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    world.Shutdown();
    return 0;
}
