//DB
#pragma once
#include <vector>
#include <iostream>
#include <unordered_map>
#include "entity.h"
#include "hierarchy.h"

// prefab entity template
struct PrefabEntity
{
	Hierarchy hierarchy;
	std::unordered_map<std::string, void*> components; // a placeholder we replace
};

// prefab definition
struct Prefab
{
	Entity rootTemplate; // template root entity
	std::unordered_map<Entity, PrefabEntity> entities; //template entities
};