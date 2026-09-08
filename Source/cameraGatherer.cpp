//DB
#include "cameraGatherer.h"
#include "camera.h"
#include "Transform.h"

void CameraGatherer::Gather(ComponentManager& components, CameraData& outData)
{
	// find active camera
	Entity cameraEntity = components.GetActiveCamera();
	if (!cameraEntity.IsValid())
		return;

	Camera* cam = components.GetCamera(cameraEntity);
	Transform* t = components.GetTransform(cameraEntity);

	if (!cam || !t)
		return;

	// fill camera data
	outData.viewMatrix = cam->viewMatrix;
	outData.projectionMatrix = cam->projectionMatrix;
	outData.position = glm::vec3(t->worldMatrix[3]);

	outData.nearPlane = cam->nearPlane;
	outData.farPlane = cam->farPlane;
	outData.fov = cam->fov;
	outData.aspect = cam->aspect;
}