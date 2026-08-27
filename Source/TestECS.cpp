#include <iostream>
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SceneGraph.h"

// test should print Entity position and the root should have 2 children

int main()
{
	EntityManager entityManager;
	ComponentManager components;
	SceneGraph sceneGraph;

	// Create entities
	Entity root = entityManager.CreateEntity();
	Entity child1 = entityManager.CreateEntity();
	Entity child2 = entityManager.CreateEntity();

	// Add transforms
	Transform rootT;
	rootT.position = { 0.0f, 0.0f, 0.0f };
	components.AddTransform(root, rootT);

	Transform child1T;
	child1T.position = { 1.0f, 0.0f, 0.0f };
	components.AddTransform(child1, child1T);

	Transform child2T;
	child2T.position = { 0.0f, 1.0f, 0.0f };
	components.AddTransform(child2, child2T);

	// Scene Graph parenting
	sceneGraph.SetParent(child1, root);
	sceneGraph.SetParent(child2, root);

	// Print transforms
	auto& transforms = components.GetAllTransforms();
	auto& entities = components.GetTransformEntities();

	for (size_t i = 0; i < transforms.size(); ++i)
	{
		const Entity& e = entities[i];
		const Transform& t = transforms[i];
		std::cout << "Entity " << e.index
				  << "pos = (" << t.position.x << " , "
							   << t.position.y << " , "
							   << t.position.z << ")\n";
	}

	// Check children of root
	const auto& children = sceneGraph.GetChildren(root);
	std::cout << "Root has " << children.size() << " children.\n";

	return 0;
}