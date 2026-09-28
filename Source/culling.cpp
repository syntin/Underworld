//DB
#include "culling.h"
#include <glm/gtc//matrix_access.hpp>

static Plane NormalizePlane(const Plane& p)
{
	float len = glm::length(p.normal);
	return { p.normal / len, p.d / len };
}

Frustum ExtractFrustum(const glm::mat4& vp)
{
	Frustum f;

	// Left
	f.planes[0].normal.x = vp[0][3] + vp[0][0];
	f.planes[0].normal.y = vp[1][3] + vp[1][0];
	f.planes[0].normal.z = vp[2][3] + vp[2][0];
	f.planes[0].d		 = vp[3][3] + vp[3][0];

	// Right
	f.planes[1].normal.x = vp[0][3] - vp[0][0];
	f.planes[1].normal.y = vp[1][3] - vp[1][0];
	f.planes[1].normal.z = vp[2][3] - vp[2][0];
	f.planes[1].d		 = vp[3][3] - vp[3][0];

	// Bottom
    f.planes[2].normal.x = vp[0][3] + vp[0][1];
    f.planes[2].normal.y = vp[1][3] + vp[1][1];
    f.planes[2].normal.z = vp[2][3] + vp[2][1];
    f.planes[2].d        = vp[3][3] + vp[3][1];

	// Top
	f.planes[3].normal.x = vp[0][3] - vp[0][1];
	f.planes[3].normal.y = vp[1][3] - vp[1][1];
	f.planes[3].normal.z = vp[2][3] - vp[2][1];
	f.planes[3].d		 = vp[3][3] - vp[3][1];

	// Near
	f.planes[4].normal.x = vp[0][3] + vp[0][2];
	f.planes[4].normal.y = vp[1][3] + vp[1][2];
	f.planes[4].normal.z = vp[2][3] + vp[2][2];
	f.planes[4].d		 = vp[3][3] + vp[3][2];

	// Far
	f.planes[5].normal.x = vp[0][3] - vp[0][2];
	f.planes[5].normal.y = vp[1][3] - vp[1][2];
	f.planes[5].normal.z = vp[2][3] - vp[2][2];
	f.planes[5].d		 = vp[3][3] - vp[3][2];

	for (int i = 0; i < 6; i++)
		f.planes[i] = NormalizePlane(f.planes[i]);

	return f;
}

bool AABBInsideFrustum(const AABB& aabb, const Frustum& f)
{
	for (int i = 0; i < 6; i++)
	{
		const Plane& p = f.planes[i];

		glm::vec3 positiveVertex = aabb.min;

		if (p.normal.x >= 0) positiveVertex.x = aabb.max.x;
		if (p.normal.y >= 0) positiveVertex.y = aabb.max.y;
		if (p.normal.z >= 0) positiveVertex.z = aabb.max.z;

		float dist = glm::dot(p.normal, positiveVertex) + p.d;

		if (dist < 0)
			return false;
	}

	return true;

}

AABB TransformAABB(const AABB& local, const glm::mat4& world)
{
	glm::vec3 corners[8] = {
		{local.min.x, local.min.y, local.min.z},
		{local.max.x, local.min.y, local.min.z},
		{local.min.x, local.max.y, local.min.z},
		{local.max.x, local.max.y, local.min.z},
		{local.min.x, local.min.y, local.max.z},
		{local.max.x, local.min.y, local.max.z},
		{local.min.x, local.max.y, local.max.z},
		{local.max.x, local.max.y, local.max.z}
	};

	AABB worldAABB;
	worldAABB.min = glm::vec3(FLT_MAX);
	worldAABB.max = glm::vec3(-FLT_MAX);

	for (int i = 0; i < 8; i++)
	{
		glm::vec3 w = glm::vec3(world * glm::vec4(corners[i], 1.0f));
		worldAABB.min = glm::min(worldAABB.min, w);
		worldAABB.max = glm::max(worldAABB.max, w);
	}

	return worldAABB;
}