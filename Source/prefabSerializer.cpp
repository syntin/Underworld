//DB
#include "prefabSerializer.h"
#include <fstream>
#include <sstream>

void PrefabSerializer::SavePrefab(const Prefab& prefab, const std::string& path)
{
	std::ofstream file(path);
	if (!file.is_open())
		return;

	file << "Prefab\n";
	file << "Root " << prefab.rootTemplate.index << " " << prefab.rootTemplate.generation << "\n\n";

	for (auto& [templateEntity, pe] : prefab.entities)
	{
		file << "Entity " << templateEntity.index << " " << templateEntity.generation << "\n";

		file << "Parent " << pe.hierarchy.parent.index << " " << pe.hierarchy.parent.generation << "\n";
		file << "FirstChild " << pe.hierarchy.firstChild.index << " " << pe.hierarchy.firstChild.generation << "\n";
		file << "NextSibling " << pe.hierarchy.nextSibling.index << " " << pe.hierarchy.nextSibling.generation << "\n";

		file << "---\n\n";
	}
}

Prefab PrefabSerializer::LoadPrefab(const std::string& path)
{
	std::ifstream file(path);
	Prefab prefab;

	if (!file.is_open())
		return prefab;

	std::string line;
	std::string token;

	// "Prefab"
	std::getline(file, line);

	// "Root index generation"
	std::getline(file, line);
	{
		std::istringstream iss(line);
		iss >> token; // Root
		iss >> prefab.rootTemplate.index >> prefab.rootTemplate.generation;
	}

	while (std::getline(file, line))
	{
		if (line.empty())
			continue;

		std::istringstream iss(line);
		iss >> token;

		if (token == "Entity")
		{
			Entity templateEntity;
			iss >> templateEntity.index >> templateEntity.generation;

			PrefabEntity pe;

			// Parent
			std::getline(file, line);
			{
				std::istringstream p(line);
				p >> token;
				p >> pe.hierarchy.parent.index >> pe.hierarchy.parent.generation;
			}

			// FirstChild
			std::getline(file, line);
			{
				std::istringstream fc(line);
				fc >> token;
				fc >> pe.hierarchy.firstChild.index >> pe.hierarchy.firstChild.generation;
			}

			// NextSibling
			std::getline(file, line);
			{
				std::istringstream ns(line);
				ns >> token;
				ns >> pe.hierarchy.nextSibling.index >> pe.hierarchy.nextSibling.generation;
			}

			// Skip "---"
			std::getline(file, line);

			prefab.entities[templateEntity] = pe;
		}
	}

	return prefab;
}