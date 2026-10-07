//DB
#pragma once
#include <glm/glm.hpp>

struct AABB {
	glm::vec3 min;
	glm::vec3 max;
};

struct BoundsComponent {
	AABB localBounds; // Mesh-space AABB
	AABB worldBounds; // World-space AABB (computed every frame)
	bool visible = true;
};

struct Plane {
	glm::vec3 normal;
	float d;
};

struct Frustum {
	Plane planes[6]; // left, right, top, bottom, near, far
};

Frustum ExtractFrustum(const glm::mat4& vp);

AABB TransformAABB(const AABB& local, const glm::mat4& world);

bool AABBInsideFrustum(const AABB& aabb, const Frustum& f);