//DB
#pragma once
#include "World.h"
#include "prefab.h"

class World;

struct SpawnParams
{
	Entity parent = {};
	bool inheritTransform = true;
};

class EntitySpawner
{
public:
	EntitySpawner(World& world);

	Entity SpawnEmpty(const SpawnParams& params = {});
	Entity SpawnPrefab(Entity prefabEntity, const SpawnParams& params = {});

private:
	World& world;
};