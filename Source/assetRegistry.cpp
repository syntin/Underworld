//DB
#include "assetRegistry.h"

AssetHandle AssetRegistry::RegisterAsset(const std::string& path, AssetType type)
{
	if (pathToHandle.contains(path))
		return pathToHandle[path];

	AssetHandle handle{ nextID++ };

	AssetMetadata meta;
	meta.handle = handle;
	meta.type = type;
	meta.path = path;
	meta.loaded = false;

	assets[handle.id] = meta;
	pathToHandle[path] = handle;

	return handle;
}

AssetMetadata* AssetRegistry::GetMetadata(AssetHandle handle)
{
	auto it = assets.find(handle.id);
	if (it == assets.end())
		return nullptr;
	return &it->second;
}

AssetHandle AssetRegistry::FindByPath(const std::string& path)
{
	if (!pathToHandle.contains(path))
		return AssetHandle{};
	return pathToHandle[path];
}