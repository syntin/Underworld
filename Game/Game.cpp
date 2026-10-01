//DB
#include "World.h"
#include "vulkanBackendAdapter.h"
#include <chrono>
#include <thread>
#include <SDL3/SDL.h>
#include "mesh.h"
#include "culling.h"

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

    MovementComponent rootMove;
    components.AddMovement(root, rootMove);

    // CHILD 1 (clockwise orbit)
    Entity child1 = entityManager.CreateEntity();

    Transform child1T;
    child1T.position = { 1, 0, 0 };
    components.AddTransform(child1, child1T);
    components.AddTriangleRenderable(child1, TriangleRenderable{});

    MovementComponent child1Move;
    components.AddMovement(child1, child1Move);

    // Mesh + Bounds for child1
    ECS::Mesh mesh1;
    mesh1.boundsMin = glm::vec3(-0.5f, -0.5f, 0.0f);
    mesh1.boundsMax = glm::vec3(0.5f, 0.5f, 0.0f);
    components.AddMesh(child1, mesh1);

    BoundsComponent bc1;
    bc1.localBounds.min = mesh1.boundsMin;
    bc1.localBounds.max = mesh1.boundsMax;
    components.AddBounds(child1, bc1);

    // CHILD 2 (counter‑clockwise orbit)
    Entity child2 = entityManager.CreateEntity();

    Transform child2T;
    child2T.position = { 0, 1, 0 };
    components.AddTransform(child2, child2T);
    components.AddTriangleRenderable(child2, TriangleRenderable{});

    MovementComponent child2Move;
    components.AddMovement(child2, child2Move);

    // Mesh + Bounds for child2
    ECS::Mesh mesh2;
    mesh2.boundsMin = glm::vec3(-0.5f, -0.5f, 0.0f);
    mesh2.boundsMax = glm::vec3(0.5f, 0.5f, 0.0f);
    components.AddMesh(child2, mesh2);

    BoundsComponent bc2;
    bc2.localBounds.min = mesh2.boundsMin;
    bc2.localBounds.max = mesh2.boundsMax;
    components.AddBounds(child2, bc2);

    // PLAYER ENTITY (WASD movement)
    Entity player = entityManager.CreateEntity();

    Transform playerT;
    playerT.position = { 0, 0, 0 };
    components.AddTransform(player, playerT);
    components.AddTriangleRenderable(player, TriangleRenderable{});

    MovementComponent playerMove;
    components.AddMovement(player, playerMove);

    // Mesh + Bounds for player
    ECS::Mesh meshPlayer;
    meshPlayer.boundsMin = glm::vec3(-0.5f, -0.5f, 0.0f);
    meshPlayer.boundsMax = glm::vec3(0.5f, 0.5f, 0.0f);
    components.AddMesh(player, meshPlayer);

    BoundsComponent bcPlayer;
    bcPlayer.localBounds.min = meshPlayer.boundsMin;
    bcPlayer.localBounds.max = meshPlayer.boundsMax;
    components.AddBounds(player, bcPlayer);

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

        MovementComponent* pm = components.GetMovement(player);

        // reset velocity each frame 
        pm->velocity = glm::vec3(0.0f);

        if (keys[SDL_SCANCODE_W])
            pm->velocity.y -= speed;
        if (keys[SDL_SCANCODE_S])
            pm->velocity.y += speed;
        if (keys[SDL_SCANCODE_A])
            pm->velocity.x -= speed;
        if (keys[SDL_SCANCODE_D])
            pm->velocity.x += speed;

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