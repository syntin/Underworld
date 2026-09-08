//DB
#include "materialGatherer.h"
#include <unordered_set>

void MaterialGatherer::Gather(const RenderList& renderList, MaterialList& outList)
{
	outList.Clear();

	std::unordered_set<Material*> uniqueMaterials;

	for (const RenderInstance& instance : renderList.instances)
	{
		Material* mat = instance.material;
		if (!mat) continue;

		if (uniqueMaterials.insert(mat).second)
		{
			outList.materials.push_back(mat);
		}
	}
}