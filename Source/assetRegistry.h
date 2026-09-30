//DB
#pragma once
#include <string>
#include <unordered_map>

struct AssetHandle
{
	uint32_t id = 0;
	bool IsValid() const { return id != 0; }
};

enum class AssetType
{
	Mesh,
	Material,
	Texture,
	Animation,
	Skin,
	Prefab,
	Scene,
	Audio,
	Script
};

struct AssetMetadata
{
	AssetHandle handle;
	AssetType type;
	std::string path;
	std::string name;
	bool loaded = false;
};

class AssetRegistry
{
public:
	AssetHandle RegisterAsset(const std::string& path, AssetType type);
	AssetMetadata* GetMetadata(AssetHandle handle);
	AssetHandle FindByPath(const std::string& path);

private:
	uint32_t nextID = 1;
	std::unordered_map<uint32_t, AssetMetadata> assets;
	std::unordered_map<std::string, AssetHandle> pathToHandle;
};