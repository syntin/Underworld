// DB
#include "scene.h"
#include "hierarchy.h"

Scene::Scene(ComponentManager& components, EntityManager& entities)
    : m_components(components), m_entities(entities)
{}

Entity Scene::CreateEntity()
{
    Entity e = m_entities.CreateEntity();

    Hierarchy h{};
    m_components.AddHierarchy(e, h);

    return e;
}

Entity Scene::CreateChildEntity(Entity parent)
{
    Entity child = m_entities.CreateEntity();

    Hierarchy h{};
    h.parent = parent;

    Hierarchy* parentH = m_components.GetHierarchy(parent);
    if (parentH)
    {
        h.nextSibling = parentH->firstChild;
        parentH->firstChild = child;
    }

    m_components.AddHierarchy(child, h);

    return child;
}

void Scene::DestroyEntityRecursive(Entity e)
{
    Hierarchy* h = m_components.GetHierarchy(e);
    if (h)
    {
        Entity child = h->firstChild;
        while (child.IsValid())
        {
            Hierarchy* ch = m_components.GetHierarchy(child);
            Entity next = (ch ? ch->nextSibling : INVALID_ENTITY);

            DestroyEntityRecursive(child);
            child = next;
        }
    }

    DestroyEntity(e);
}

void Scene::DestroyEntity(Entity e)
{
    Hierarchy* h = m_components.GetHierarchy(e);
    if (h && h->parent.IsValid())
    {
        Hierarchy* parentH = m_components.GetHierarchy(h->parent);
        if (parentH)
        {
            Entity* cur = &parentH->firstChild;
            while (cur->IsValid())
            {
                if (*cur == e)
                {
                    Hierarchy* curH = m_components.GetHierarchy(*cur);
                    *cur = (curH ? curH->nextSibling : INVALID_ENTITY);
                    break;
                }
                Hierarchy* curH = m_components.GetHierarchy(*cur);
                cur = (curH ? &curH->nextSibling : nullptr);
                if (!cur) break;
            }
        }
    }

    // Remove all components for this entity
    m_components.RemoveHierarchy(e);
    m_components.RemoveTransform(e);
    m_components.RemoveMesh(e);
    m_components.RemoveMaterial(e);
    m_components.RemoveVelocity(e);
    m_components.RemoveHealth(e);
    m_components.RemoveCollider(e);
    m_components.RemoveLight(e);
    m_components.RemoveScript(e);
    m_components.RemoveAudio(e);
    m_components.RemoveAnimation(e);
    m_components.RemoveSkeleton(e);
    m_components.RemoveAnimationClip(e);
    m_components.RemoveAnimator(e);

    m_entities.DestroyEntity(e);
}

void Scene::DestroyAllEntities()
{
    auto all = m_entities.GetAllEntities();
    for (Entity e : all)
        DestroyEntityRecursive(e);
}

std::vector<Entity> Scene::GetAllEntities() const
{
    return m_entities.GetAllEntities();
}