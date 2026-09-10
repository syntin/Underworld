//DB
#include "World.h"
#include "worldSpawn.h"

EntitySpawner::EntitySpawner(World& w)
	: world(w)
{
}

Entity EntitySpawner::SpawnEmpty(const SpawnParams& params)
{
	auto& em = world.GetEntityManager();
	auto& cm = world.GetComponentManager();
	auto& sg = world.GetSceneGraph();

	Entity e = em.CreateEntity();

	Transform t;
	t.position = { 0,0,0 };
	t.worldPosition = { 0,0,0 };
	cm.AddTransform(e, t);

	if (params.parent.index != 0 || params.parent.generation != 0)
		sg.SetParent(e, params.parent);

	return e;
}

Entity EntitySpawner::SpawnPrefab(Entity prefabEntity, const SpawnParams& params)
{
    auto& em = world.GetEntityManager();
    auto& cm = world.GetComponentManager();
    auto& sg = world.GetSceneGraph();

    Entity e = em.CreateEntity();

    // Copy components from prefab
    if (cm.HasTransform(prefabEntity))
    {
        Transform* pt = cm.GetTransform(prefabEntity);
        Transform t = *pt;
        cm.AddTransform(e, t);
    }

    if (cm.HasMesh(prefabEntity))
    {
        ECS::Mesh* pm = cm.GetMesh(prefabEntity);
        cm.AddMesh(e, *pm);
    }

    if (cm.HasMaterial(prefabEntity))
    {
        Material* pm = cm.GetMaterial(prefabEntity);
        cm.AddMaterial(e, *pm);
    }

    if (cm.HasAnimator(prefabEntity))
    {
        Animator* pa = cm.GetAnimator(prefabEntity);
        cm.AddAnimator(e, *pa);
    }

    if (cm.HasAnimation(prefabEntity))
    {
        Animation* pa = cm.GetAnimation(prefabEntity);
        cm.AddAnimation(e, *pa);
    }

    // Parent if requested
    if (params.parent.index != 0 || params.parent.generation != 0)
        sg.SetParent(e, params.parent);

    return e;
}