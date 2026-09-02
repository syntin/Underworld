#pragma once
#include <cstdint>
#include <glm/glm.hpp>

namespace ECS {

struct Mesh
	{
		// CPU-side mesh reference
		uint32_t meshID = 0;

		// GPU-side buffers (we don't have these yet)
		void* vertexBuffer = nullptr;
		void* indexBuffer = nullptr;

		uint32_t indexCount = 0;

		// Material reference (we don't have this yet either)
		uint32_t materialID = 0;

		// Bounding box (we don't use this yet but we probably will need it)
		glm::vec3 boundsMin = glm::vec3(0.0f);
		glm::vec3 boundsMax = glm::vec3(0.0f);

		bool dirty = true;
	};

}

class Mesh {
public:
	Mesh();
	virtual ~Mesh();
	void Initialize();
};
