//DB
#pragma once

#include <vector>
#include "World.h"

struct PotentialCollision
{
	Entity a;
	Entity b;
};

class PhysicsBroadphaseSystem
{
public:
	void Update(World& world);

	const std::vector<PotentialCollision>& GetPairs() const { return pairs; }

private:
	std::vector<PotentialCollision> pairs;
};