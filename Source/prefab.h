//DB
#pragma once
#include <variant>
#include <string>
#include <unordered_map>

#include "entity.h"
#include "hierarchy.h"
#include "Transform.h"
#include "mesh.h"
#include "material.h"
#include "velocity.h"
#include "healthComponents.h"
#include "lightComponents.h"
#include "colliderComponents.h"
#include "animationStateComponents.h"

using ComponentVariant = std::variant<
	Transform,
	ECS::Mesh,
	Material,
	Velocity,
	Health,
	Light,
	Collider,
	Animation
>;

struct PrefabEntity
{
	Hierarchy hierarchy;
	std::unordered_map<std::string, ComponentVariant> components;
};

// prefab definition
struct Prefab
{
	Entity rootTemplate; // template root entity
	std::unordered_map<Entity, PrefabEntity> entities; //template entities
};