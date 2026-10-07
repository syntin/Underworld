//DB
#pragma once
#include "physicsContact.h"
#include <vector>

class World;

class RigidBodySolverSystem
{
public:
	void Update(World& world, float dt);
};