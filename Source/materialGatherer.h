//DB
#pragma once
#include "materialList.h"
#include "renderList.h"

class MaterialGatherer
{
public:
	void Gather(const RenderList& renderList, MaterialList& outList);
};