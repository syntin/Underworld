//DB
#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "renderList.h"
#include "materialList.h"
#include "cameraData.h"
#include "lightList.h"
#include "boneMatrixList.h"
#include "entity.h"

// Per-object data 
struct ObjectGPUData
{
	glm::mat4 worldMatrix;
	uint32_t materialIndex;
	uint32_t meshIndex;
	Entity entity;
};

// Per-material data
struct MaterialGPUData
{
	glm::vec3 albedo;
	float roughness;
	float metallic;
	uint32_t albedoTextureID;
	uint32_t normalTextureID;
	uint32_t metallicTextureID;
	uint32_t roughbessTextureID;

	uint32_t pipelineID;
};

// Per-light data
struct LightGPUData
{
	glm::vec3 position;
	float intensity;

	glm::vec3 direction;
	float range;

	glm::vec3 color;
	float spotAngle;

	int type; // 0 = dir, 1 = point, 2 = spot
};

// Packed per-frame data
struct PerFrameCPUData
{
	//Camera
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::vec3 cameraPosition;
	float pad0;

	// Objects
	std::vector<ObjectGPUData> objects;

	// Materials
	std::vector<MaterialGPUData> materials;

	// Lights
	std::vector<LightGPUData> lights;

	// Skinning
	std::vector<glm::mat4> skinMatrices;
};

class PerFrameBufferBuilder
{
public:
	void Build(
		const RenderList& renderList,
		const MaterialList& materialList,
		const CameraData& cameraData,
		const LightList& lightList,
		const BoneMatrixList& boneMatrixList,
		PerFrameCPUData& outData);
};