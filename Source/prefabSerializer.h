//DB
#pragma once
#include <string>
#include "prefab.h"

class PrefabSerializer
{
public:
	static void SavePrefab(const Prefab& prefab, const std::string& path);
	static Prefab LoadPrefab(const std::string& path);
};