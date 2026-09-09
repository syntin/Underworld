//DB
#pragma once
#include <vector>
#include "entity.h"
#include "componentManager.h"
#include "entityManager.h"

class Scene
{
public:
    Scene(ComponentManager & components, EntityManager & entities);

    // Create a root entity (no parent)
    Entity CreateEntity();

    // Create a child entity under a parent
    Entity CreateChildEntity(Entity parent);

    // Destroy a single entity (no recursion)
    void DestroyEntity(Entity e);

    // Destroy an entity and all children
    void DestroyEntityRecursive(Entity e);

    // Destroy entire scene
    void DestroyAllEntities();

    // Get all entities in the scene
    std::vector<Entity> GetAllEntities() const;

    ComponentManager& GetComponents() { return m_components; }
    EntityManager& GetEntities() { return m_entities; }

private:
    ComponentManager& m_components;
    EntityManager& m_entities;
};