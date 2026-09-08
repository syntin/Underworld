// DB
#include "scene.h"
#include "hierarchy.h"

Scene::Scene(ComponentManager& components, EntityManager& entities)
	: m_components(components), m_entities(entities)
{}

Entity Scene::CreateEntity()
{
	// Create entity using entityManager
	Entity e = m_entities.CreateEntity();

	// Every entity gets a hierarchy component
	Hierarchy h{};
	m_components.AddHierarchy(e, h);

	return e;
}

Entity Scene::CreateChildEntity(Entity parent)
{
	Entity child = m_entities.CreateEntity();

	Hierarchy h{};
	h.parent = parent;

	// Insert child at the front of the parent's child list
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
		// Destroy children first
		Entity child = h->firstChild;
		while (child.index != 0 || child.generation != 0) // child != INVALID_ENTITY
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
		// Remove from parent's child list
		Hierarchy* parentH = m_components.GetHierarchy(h->parent);
		if (parentH)
		{
			Entity* cur = &parentH->firstChild;
			while (cur->IsValid())
			{
				if (*cur == e)
				{
					*cur = m_components.GetHierarchy(*cur)->nextSibling;
					break;
				}
				cur = &m_components.GetHierarchy(*cur)->nextSibling;
			}
		}
	}

	// Remove all components for this entity
}
