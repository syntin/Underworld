//DB
#include "cameraControllerSystem.h"
#include "componentManager.h"

void CameraControllerSystem::Update(World& world, float dt)
{
	auto& components = world.GetComponentManager();
	auto& cameras = components.GetCameraEntities();

	for (auto& entity : cameras)
	{
		Transform* t = components.GetTransform(entity);
		Camera* cam = components.GetCamera(entity);

		if (!t || !cam)
			continue;

	}
}