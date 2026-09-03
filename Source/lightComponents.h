#pragma once
#include <glm/glm.hpp>

enum class LightType
{
	Directional,
	Point,
	Spot
};

struct Light
{
	LightType type = LightType::Point;

	glm::vec3 color = glm::vec3(1.0f);
	float intensity = 1.0f;

	float range = 10.0f; // for point/spot
	float spotAngle = 45.0f; // degrees for spotlights

	bool dirty = true;
};

class LightComponents
{
public:
	LightComponents();
	virtual ~LightComponents();

public:
	void Initialize();
};