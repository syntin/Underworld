//DB
#include "prefabManager.h"
#include "prefabSerializer.h"

PrefabManager::PrefabManager(Scene& scene, ComponentManager& components, EntityManager& entities)
    : m_scene(scene), m_components(components), m_entities(entities)
{}

int PrefabManager::CreatePrefab(Entity root)
{
    Prefab prefab;
    prefab.rootTemplate = root;

    std::vector<Entity> stack;
    stack.push_back(root);

    while (!stack.empty())
    {
        Entity e = stack.back();
        stack.pop_back();

        PrefabEntity pe;
        pe.hierarchy = *m_components.GetHierarchy(e);

        // Store components into the prefab
        if (m_components.HasTransform(e))
            pe.components["Transform"] = *m_components.GetTransform(e);

        if (m_components.HasVelocity(e))
            pe.components["Velocity"] = *m_components.GetVelocity(e);

        if (m_components.HasHealth(e))
            pe.components["Health"] = *m_components.GetHealth(e);

        if (m_components.HasLight(e))
            pe.components["Light"] = *m_components.GetLight(e);

        if (m_components.HasCollider(e))
            pe.components["Collider"] = *m_components.GetCollider(e);

        if (m_components.HasMaterial(e))
            pe.components["Material"] = *m_components.GetMaterial(e);

        if (m_components.HasMesh(e))
            pe.components["Mesh"] = *m_components.GetMesh(e);

        if (m_components.HasAnimation(e))
            pe.components["Animation"] = *m_components.GetAnimation(e);

        prefab.entities[e] = pe;

        // Push children
        Hierarchy* h = m_components.GetHierarchy(e);
        Entity child = h->firstChild;

        while (child.IsValid())
        {
            stack.push_back(child);
            Hierarchy* ch = m_components.GetHierarchy(child);
            child = ch->nextSibling;
        }
    }

    int id = m_nextPrefabID++;
    m_prefabs[id] = prefab;
    return id;
}

Entity PrefabManager::Instantiate(int prefabID)
{
    Prefab& prefab = m_prefabs[prefabID];

    std::unordered_map<Entity, Entity> map;

    // Create root
    Entity newRoot = m_scene.CreateEntity();
    map[prefab.rootTemplate] = newRoot;

    // Create children
    for (auto& [templateEntity, pe] : prefab.entities)
    {
        if (templateEntity == prefab.rootTemplate)
            continue;

        Entity parentTemplate = pe.hierarchy.parent;
        Entity parentNew = map[parentTemplate];

        Entity newEntity = m_scene.CreateChildEntity(parentNew);
        map[templateEntity] = newEntity;
    }

    // Apply serialized components
    for (auto& [templateEntity, pe] : prefab.entities)
    {
        Entity newEntity = (templateEntity == prefab.rootTemplate)
            ? newRoot
            : map[templateEntity];

        CopyEntityComponents(pe, newEntity);
    }

    return newRoot;
}

void PrefabManager::Save(int prefabID, const std::string& path)
{
    PrefabSerializer::SavePrefab(m_prefabs[prefabID], path);
}

int PrefabManager::Load(const std::string& path)
{
    Prefab prefab = PrefabSerializer::LoadPrefab(path);
    int id = m_nextPrefabID++;
    m_prefabs[id] = prefab;
    return id;
}

void PrefabManager::CopyEntityComponents(const PrefabEntity& pe, Entity dst)
{
    for (auto& [name, comp] : pe.components)
    {
        if (name == "Transform" && std::holds_alternative<Transform>(comp))
            m_components.AddTransform(dst, std::get<Transform>(comp));

        else if (name == "Velocity" && std::holds_alternative<Velocity>(comp))
            m_components.AddVelocity(dst, std::get<Velocity>(comp));

        else if (name == "Health" && std::holds_alternative<Health>(comp))
            m_components.AddHealth(dst, std::get<Health>(comp));

        else if (name == "Light" && std::holds_alternative<Light>(comp))
            m_components.AddLight(dst, std::get<Light>(comp));

        else if (name == "Collider" && std::holds_alternative<Collider>(comp))
            m_components.AddCollider(dst, std::get<Collider>(comp));

        else if (name == "Material" && std::holds_alternative<Material>(comp))
            m_components.AddMaterial(dst, std::get<Material>(comp));

        else if (name == "Mesh" && std::holds_alternative<ECS::Mesh>(comp))
            m_components.AddMesh(dst, std::get<ECS::Mesh>(comp));

        else if (name == "Animation" && std::holds_alternative<Animation>(comp))
            m_components.AddAnimation(dst, std::get<Animation>(comp));
    }
}