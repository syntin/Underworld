//DB
#pragma once
#include "entity.h"
#include "mesh.h"
#include "material.h"
#include <glm/glm.hpp>

struct RenderInstance
{
	Entity entity;
	ECS::Mesh* mesh;
	Material* material;
	glm::mat4 worldMatrix;
};