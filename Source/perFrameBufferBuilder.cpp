//DB
#include "perFrameBufferBuilder.h"

void PerFrameBufferBuilder::Build(
	const RenderList& renderList,
	const MaterialList& materialList,
	const CameraData& cameraData,
	const LightList& lightList,
	const BoneMatrixList& boneMatrixList,
	PerFrameCPUData& outData)
{
	//Camera
	outData.viewMatrix = cameraData.viewMatrix;
	outData.projectionMatrix = cameraData.projectionMatrix;
	outData.cameraPosition = cameraData.position;
	outData.pad0 = 0.0f;

	// Materials
	outData.materials.clear();
	outData.materials.reserve(materialList.materials.size());

	for (Material* mat : materialList.materials)
	{
		if (!mat) continue;

		MaterialGPUData gpuMat;
		gpuMat.albedo			  = mat->albedo;
		gpuMat.roughness		  = mat->roughness;
		gpuMat.metallic			  = mat->metallic;
		gpuMat.albedoTextureID    = mat->albedoTextureID;
		gpuMat.normalTextureID	  = mat->normalTextureID;
		gpuMat.metallicTextureID  = mat->metallicTextureID;
		gpuMat.roughbessTextureID = mat->roughnessTextureID;
		gpuMat.pipelineID		  = mat->pipelineID;

		outData.materials.push_back(gpuMat);
	}

	// Objects

	outData.objects.clear();
	outData.objects.reserve(renderList.instances.size());

	for (const RenderInstance& instance : renderList.instances)
	{
		ObjectGPUData obj{};
		obj.worldMatrix = instance.worldMatrix;
		obj.entity = instance.entity;

		// Find material in materialList
		uint32_t matIndex = 0;
		for (uint32_t i = 0; i < (uint32_t)materialList.materials.size(); i++)
		{
			if (materialList.materials[i] == instance.material)
			{
				matIndex = i;
				break;
			}
		}

		obj.materialIndex = matIndex;
		obj.meshIndex = instance.mesh ? instance.mesh->meshID : 0;

		outData.objects.push_back(obj);
	}

	//Lights
	outData.lights.clear();
	outData.lights.reserve(lightList.lights.size());

	for (const LightData& ld : lightList.lights)
	{
		LightGPUData gpuLight{};
		gpuLight.position	= ld.position;
		gpuLight.intensity	= ld.intensity;
		gpuLight.direction	= ld.direction;
		gpuLight.range		= ld.range;
		gpuLight.color		= ld.color;
		gpuLight.spotAngle	= ld.spotAngle;
		gpuLight.type		= ld.type;

		outData.lights.push_back(gpuLight);
	}

	// Skinning
	outData.skinMatrices.clear();

	for (const BoneMatrixEntry& entry : boneMatrixList.entries)
	{
		for (const glm::mat4& m : entry.matrices);
	}
}