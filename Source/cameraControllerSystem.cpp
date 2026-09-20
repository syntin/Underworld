//DB
#include "cameraControllerSystem.h"
#include "componentManager.h"
#include "input.h"

void CameraControllerSystem::Update(World& world, float dt)
{
    auto& components = world.GetComponentManager();
    auto& entities = components.GetCameraControllerEntities();
    auto& input = world.GetInput();

    for (auto& entity : entities)
    {
        Transform* t = components.GetTransform(entity);
        Camera* cam = components.GetCamera(entity);
        CameraControllerComponent* ctrl = components.GetCameraController(entity);

        if (!t || !cam || !ctrl)
            continue;

        glm::vec3 move(0);

        if (input.IsKeyDown(KEY_W)) move.z += 1;
        if (input.IsKeyDown(KEY_S)) move.z -= 1;
        if (input.IsKeyDown(KEY_A)) move.x -= 1;
        if (input.IsKeyDown(KEY_D)) move.x += 1;

        if (move != glm::vec3(0))
        {
            float speed = ctrl->moveSpeed * dt;

            // Compute forward/right from rotation
            float pitch = glm::radians(t->rotation.x);
            float yaw = glm::radians(t->rotation.y);

            glm::vec3 forward;
            forward.x = cos(yaw) * cos(pitch);
            forward.y = sin(pitch);
            forward.z = sin(yaw) * cos(pitch);

            glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));

            t->position += forward * move.z * speed;
            t->position += right * move.x * speed;

            t->dirty = true;
            cam->dirty = true;
        }
    }
}