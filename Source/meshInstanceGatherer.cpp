//DB
#include "meshInstanceGatherer.h"
#include "componentManager.h"
#include "Transform.h"
#include "mesh.h"
#include "material.h"

void MeshInstanceGatherer::Gather(ComponentManager& components, RenderList& outList)
{
	outList.Clear();

	auto& meshEntities = components.GetMeshEntities();

	for (Entity e : meshEntities)
	{
		ECS::Mesh* mesh = components.GetMesh(e);
		if (!mesh) continue;

		Transform* t = components.GetTransform(e);
		if (!t) continue;

		Material* mat = components.GetMaterial(e);
		
		RenderInstance instance;
		instance.entity = e;
		instance.mesh = mesh;
		instance.material = mat;
		instance.worldMatrix = t->worldMatrix;

		outList.instances.push_back(instance);
	}
}