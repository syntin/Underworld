//DB
#pragma once
#include <string>
#include "prefabManager.h"

class PrefabSerializer
{
public:
	static void SavePrefab(const Prefab& prefab, const std::string& path);
	static Prefab LoadPrefab(const std::string& path);
};