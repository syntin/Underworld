//DB
#include "world.h"
#include "vulkanBackendAdapter.h"
#include <chrono>
#include <thread>

int Game()
{
    World world;
    world.Initialize();

    VulkanBackendAdapter backend;
    backend.Initialize();

    auto& entityManager = world.GetEntityManager();
    auto& components = world.GetComponentManager();
    auto& sceneGraph = world.GetSceneGraph();

    Entity root = entityManager.CreateEntity();
    Entity child1 = entityManager.CreateEntity();
    Entity child2 = entityManager.CreateEntity();

    Transform rootT;
    rootT.position = { 0, 0, 0 };
    components.AddTransform(root, rootT);

    Transform child1T;
    child1T.position = { 1, 0, 0 };
    components.AddTransform(child1, child1T);

    Transform child2T;
    child2T.position = { 0, 1, 0 };
    components.AddTransform(child2, child2T);

    sceneGraph.SetParent(child1, root);
    sceneGraph.SetParent(child2, root);

    bool running = true;

    while (running)
    {
        world.Update(0.016f);

        Transform* t1 = components.GetTransform(child1);
        Transform* t2 = components.GetTransform(child2);

        std::cout << "Child1 Pos: "
            << t1->position.x << ", "
            << t1->position.y << ", "
            << t1->position.z << std::endl;

        std::cout << "Child2 Pos: "
            << t2->position.x << ", "
            << t2->position.y << ", "
            << t2->position.z << std::endl;

        backend.RenderFrame();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    world.Shutdown();
    return 0;
}

int main()
{
    return Game();
}
