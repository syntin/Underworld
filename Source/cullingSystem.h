//DB
#pragma once
#include "culling.h"
#include "componentManager.h"
#include "camera.h"
#include "Transform.h"
#include "World.h"

class CullingSystem {
public:
	void Update(World& world);
};