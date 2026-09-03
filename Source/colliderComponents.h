#pragma once
#include <glm/glm.hpp>

class ColliderComponents
{
public:
	ColliderComponents();
	virtual ~ColliderComponents();

public:
	void Initialize();
};

enum class ColliderType
{
	AABB,
	Sphere
};

struct Collider
{
	ColliderType type = ColliderType::AABB;

	glm::vec3 offset = glm::vec3(0.0f); // local offset from transform
	glm::vec3 halfExtents = glm::vec3(0.5f); // fir AABB
	float radius = 0.5f; //for sphere

	bool isTrigger = false;
	bool dirty = true;
};