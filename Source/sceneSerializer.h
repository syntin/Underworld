//DB
#pragma once
#include <string>

class Scene;

class PrefabManager;

class SceneSerializer
{
public:
	static void SaveScene(Scene& scene, const std::string& path);
	static void LoadScene(Scene& scene, const std::string& path);
};