//DB
#include "prefabManager.h"

PrefabManager::PrefabManager(Scene& scene, ComponentManager& components, EntityManager& entities)
	: m_scene(scene), m_components(components), m_entities(entities)
{
}

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

		prefab.entities[e] = pe;

		//Push children
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

	// Map template entities to new entities
	std::unordered_map<Entity, Entity> map;

	// Create root
	Entity newRoot = m_scene.CreateEntity();
	map[prefab.rootTemplate] = newRoot;

	// Create all children
	for (auto& [templateEntity, pe] : prefab.entities)
	{
		if (templateEntity == prefab.rootTemplate)
			continue;

		Entity parentTemplate = pe.hierarchy.parent;
		Entity parentNew = map[parentTemplate];

		Entity newEntity = m_scene.CreateChildEntity(parentNew);
		map[templateEntity] = newEntity;
	}

	// Copy components
	for (auto& [templateEntity, pe] : prefab.entities)
	{
		Entity newEntity = map[templateEntity];
		CopyEntityComponents(templateEntity, newEntity);
	}

	return newRoot;
}

void PrefabManager::CopyEntityComponents(Entity src, Entity dst)
{
	// Transform
	if (m_components.HasTransform(src))
		m_components.AddTransform(dst, *m_components.GetTransform(src));

	// Mesh
	if (m_components.HasMesh(src))
		m_components.AddMesh(dst, *m_components.GetMesh(src));

	// Material
	if (m_components.HasMaterial(src))
		m_components.AddMaterial(dst, *m_components.GetMaterial(src));

	// Velocity
	if (m_components.HasVelocity(src))
		m_components.AddVelocity(dst, *m_components.GetVelocity(src));

	// Health
	if (m_components.HasHealth(src))
		m_components.AddHealth(dst, *m_components.GetHealth(src));

	// Collider
	if (m_components.HasCollider(src))
		m_components.AddCollider(dst, *m_components.GetCollider(src));

	// Light
	if (m_components.HasLight(src))
		m_components.AddLight(dst, *m_components.GetLight(src));

	// Script
	if (m_components.HasScript(src))
		m_components.AddScript(dst, *m_components.GetScript(src));

	// Audio
	if (m_components.HasAudio(src))
		m_components.AddAudio(dst, *m_components.GetAudio(src));

	// Animation
	if (m_components.HasAnimation(src))
		m_components.AddAnimation(dst, *m_components.GetAnimation(src));

	// Skeleton
	if (m_components.HasSkeleton(src))
		m_components.AddSkeleton(dst, *m_components.GetSkeleton(src));

	// AnimationClip 
	if (m_components.HasAnimationClip(src))
		m_components.AddAnimationClip(dst, *m_components.GetAnimationClip(src));

	// Animator
	if (m_components.HasAnimator(src))
		m_components.AddAnimator(dst, *m_components.GetAnimator(src));
}