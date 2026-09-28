//DB
#include "cullingSystem.h"
#include "World.h" 
#include "culling.h"
#include "componentManager.h"
#include "camera.h"
#include "Transform.h"

void CullingSystem::Update(World& world)
{
	auto& components = world.GetComponentManager();
    auto& entities = components.GetEntitiesWithBounds();

    Camera* cam = components.GetActiveCameraComponent();
    if (!cam) return;

    glm::mat4 vp = cam->projectionMatrix * cam->viewMatrix;
    Frustum frustum = ExtractFrustum(vp);

    for (Entity e : entities)
    {
        BoundsComponent* bc = components.GetBounds(e);
        Transform* t = components.GetTransform(e);

        if (!bc || !t)
            continue;

        AABB worldAABB = TransformAABB(bc->localBounds, t->worldMatrix);

        bc->visible = AABBInsideFrustum(worldAABB, frustum);
    }
}