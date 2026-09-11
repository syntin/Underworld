//DB
#pragma once
#include "World.h"

class RuntimeScene
{
public:
	virtual ~RuntimeScene() {}

	virtual void OnEnter(World& world) = 0;
	virtual void OnExit(World& world) = 0;
	virtual void Update(World& world, float dt) = 0;
};