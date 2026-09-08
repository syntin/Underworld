//DB
#pragma once
#include "entity.h"

struct Hierarchy
{
	Entity parent = INVALID_ENTITY;
	Entity firstChild = INVALID_ENTITY;
	Entity nextSibling = INVALID_ENTITY;
};