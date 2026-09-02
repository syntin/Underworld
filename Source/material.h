#pragma once
#include <cstdint>
#include <glm/glm.hpp>

struct Material
{
	uint32_t materialID = 0;

	// PBR values (we will probably use these later)
	glm::vec3 albedo = glm::vec3(1.0f);
	float roughness = 0.5f;
	float metallic = 0.0f;

	// Texture references (same as above)
	uint32_t albedoTextureID = 0;
	uint32_t normalTextureID = 0;
	uint32_t metallicTextureID = 0;
	uint32_t roughnessTextureID = 0;

	// GPU pipeline reference (same as above above)
	uint32_t pipelineID = 0;

	bool dirty = true;
};