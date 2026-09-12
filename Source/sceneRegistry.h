//DB
#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include "runtimeScene.h"
#include "World.h"

class SceneRegistry
{
public:
	using SceneFactory = std::function<RuntimeScene*()> ;

	static void Register(const std::string& name, SceneFactory factory)
	{
		GetMap()[name] = factory;
	}

	static RuntimeScene* Create(const std::string& name)
	{
		auto& map = GetMap();
		auto it = map.find(name);
		if (it != map.end())
			return it->second();
		return nullptr;
	}

private:
	static std::unordered_map<std::string, SceneFactory>& GetMap()
	{
		static std::unordered_map<std::string, SceneFactory> map;
		return map;
	}
};