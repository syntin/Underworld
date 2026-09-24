//DB
#include "World.h"
#include "vulkanBackendAdapter.h"
#include <chrono>
#include <thread>
#include <SDL3/SDL.h>

int Game()
{
    SDL_Window* window = SDL_CreateWindow(
        "Vulkan Learning",
        1280,
        720,
        SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE
    );

    World world;
    world.Initialize();

    VulkanBackendAdapter backend;
    backend.SetWorld(&world);
    backend.Initialize(window);

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
    components.AddTriangleRenderable(child1, TriangleRenderable{});

    Transform child2T;
    child2T.position = { 0, 1, 0 };
    components.AddTransform(child2, child2T);
    components.AddTriangleRenderable(child2, TriangleRenderable{});

    sceneGraph.SetParent(child1, root);
    sceneGraph.SetParent(child2, root);

    bool running = true;

    Transform* t1 = components.GetTransform(child1);
    Transform* t2 = components.GetTransform(child2);

    backend.SetRunning();

    while (running && backend.IsRunning())
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_EVENT_WINDOW_RESIZED)
            {
                // forward to backend so it can recreate swapchain
                backend.OnWindowResize(event.window.data1, event.window.data2);
            }
        }

        world.Update(0.016f);

        //  movement 
        static float time = 0.0f;
        time += 0.016f;

        // clockwise (child1)
        t1->position.x = cos(time) * 0.5f;
        t1->position.y = sin(time) * 0.5f;

        // counter‑clockwise (child2)
        t2->position.x = cos(time) * 0.5f;
        t2->position.y = -sin(time) * 0.5f;

        
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

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

int main()
{
    return Game();
}
