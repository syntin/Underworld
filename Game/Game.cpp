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

    // ROOT ENTITY
    Entity root = entityManager.CreateEntity();

    Transform rootT;
    rootT.position = { 0, 0, 0 };
    components.AddTransform(root, rootT);

    // CHILD 1 (clockwise orbit)
    Entity child1 = entityManager.CreateEntity();

    Transform child1T;
    child1T.position = { 1, 0, 0 };
    components.AddTransform(child1, child1T);
    components.AddTriangleRenderable(child1, TriangleRenderable{});

    // CHILD 2 (counter‑clockwise orbit)
    Entity child2 = entityManager.CreateEntity();

    Transform child2T;
    child2T.position = { 0, 1, 0 };
    components.AddTransform(child2, child2T);
    components.AddTriangleRenderable(child2, TriangleRenderable{});

    // PLAYER ENTITY (WASD movement)
    Entity player = entityManager.CreateEntity();

    Transform playerT;
    playerT.position = { 0, 0, 0 };
    components.AddTransform(player, playerT);
    components.AddTriangleRenderable(player, TriangleRenderable{});

    // SCENE GRAPH PARENTING
    sceneGraph.SetParent(child1, root);
    sceneGraph.SetParent(child2, root);
    sceneGraph.SetParent(player, root);

    // TRANSFORM POINTERS
    Transform* t1 = components.GetTransform(child1);
    Transform* t2 = components.GetTransform(child2);
    Transform* playerTransform = components.GetTransform(player);

    bool running = true;
    backend.SetRunning();

    float time = 0.0f;

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
                backend.OnWindowResize(event.window.data1, event.window.data2);
            }
        }

        // FIXED TIMESTEP
        float dt = 0.016f;
        world.Update(dt);

        // ORBITING MOVEMENT
        time += dt;

        // clockwise (child1)
        t1->position.x = cos(time) * 0.5f;
        t1->position.y = sin(time) * 0.5f;

        // counter‑clockwise (child2)
        t2->position.x = cos(time) * 0.5f;
        t2->position.y = -sin(time) * 0.5f;

        // PLAYER MOVEMENT (WASD)
        const bool* keys = SDL_GetKeyboardState(NULL);
        float speed = 1.0f;

        if (keys[SDL_SCANCODE_W])
            playerTransform->position.y -= speed * dt;
        if (keys[SDL_SCANCODE_S])
            playerTransform->position.y += speed * dt;
        if (keys[SDL_SCANCODE_A])
            playerTransform->position.x -= speed * dt;
        if (keys[SDL_SCANCODE_D])
            playerTransform->position.x += speed * dt;

        // DEBUG OUTPUTS
        std::cout << "Child1 Pos: "
            << t1->position.x << ", "
            << t1->position.y << ", "
            << t1->position.z << std::endl;

        std::cout << "Child2 Pos: "
            << t2->position.x << ", "
            << t2->position.y << ", "
            << t2->position.z << std::endl;

        std::cout << "Player Pos: "
            << playerTransform->position.x << ", "
            << playerTransform->position.y << ", "
            << playerTransform->position.z << std::endl;

        // RENDER FRAME
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