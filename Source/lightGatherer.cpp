//DB
#include "lightGatherer.h"
#include "lightComponents.h"
#include "Transform.h"

void LightGatherer::Gather(ComponentManager& components, LightList& outList)
{
	outList.Clear();

	auto& lightEntities = components.GetLightEntities();

	for (Entity e : lightEntities)
	{
		Light* light = components.GetLight(e);
		Transform* t = components.GetTransform(e);

		if (!light || !t)
			continue;

		LightData data;

		// World-space position
		data.position = glm::vec3(t->worldMatrix[3]);

		// World-space direction (forward vector)
		data.direction = glm::normalize(glm::vec3(t->worldMatrix * glm::vec4(0, 0, -1, 0)));

		data.color = light->color;
		data.intensity = light->intensity;

		data.range = light->range;
		data.spotAngle = light->spotAngle;

		//convert enum to int
		switch (light->type)
		{
			case LightType::Directional: data.type = 0; break;
			case LightType::Point:		 data.type = 1; break;
			case LightType::Spot:		 data.type = 2; break;
		}

		outList.lights.push_back(data);
	}
}