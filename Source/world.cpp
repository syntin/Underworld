//DB
#include "World.h"
#include <iostream>

World::World()
    : spawner(*this)
{
}

void World::Initialize()
{
	std::cout << "World initialized.\n";
}

void World::Update(float dt)
{
    input.Update();
    UpdateTransforms();
	//add shit here later
}

void World::Shutdown()
{
	std::cout << "world shutdown.\n";
}

void World::UpdateTransforms()
{
    auto& transforms = componentManager.GetAllTransforms();
    auto& entities = componentManager.GetTransformEntities();
    auto& sceneGraph = this->sceneGraph;

    for (size_t i = 0; i < transforms.size(); ++i)
    {
        Entity e = entities[i];
        Transform& t = transforms[i];

        // Get parent
        Entity parent = sceneGraph.GetParent(e);

        // If parent is valid (not default Entity{})
        if (parent.index != 0 || parent.generation != 0)
        {
            Transform* pt = componentManager.GetTransform(parent);

            // Parent exists but has no transform
            if (pt == nullptr)
            {
                t.worldPosition = t.position;
                continue;
            }

            t.worldPosition = {
                t.position.x + pt->worldPosition.x,
                t.position.y + pt->worldPosition.y,
                t.position.z + pt->worldPosition.z
            };
        }
        else
        {
            // Root entity
            t.worldPosition = t.position;
        }
    }
}