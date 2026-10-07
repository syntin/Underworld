//DB
#pragma once
#include <vector>
#include "physicsContact.h"

class World;

class PhysicsNarrowphaseSystem
{
public:
	void Update(World& world);

	const std::vector<PhysicsContact>& GetContacts() const { return contacts; }

private:
	std::vector<PhysicsContact> contacts;
};