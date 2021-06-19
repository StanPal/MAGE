#pragma once

#include "Material.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "Texture.h"
#include "Skeleton.h"
#include "SkeletonIO.h"
#include "AnimationSet.h"
namespace MAGE::Graphics
{
	class Model;
	class ModelLoader
	{
	public:
		static void LoadModel(std::filesystem::path filename,Model& model);
		static void LoadSkeleton(std::filesystem::path fileName, Skeleton& skeleton);
		static void LoadAnimationSet(std::filesystem::path fileName, AnimationSet& skeleton);

	};
	class Model
	{
	public:
		void Initialize(const std::filesystem::path& fileName);
		void Terminate();
		
		void Draw() const;


	public:
		//Cpu Data
		struct MeshData
		{
			SkinnedMesh mesh; 
			uint32_t materialIndex = 0;
			MeshBuffer meshBuffer; 
		};

		//GpuData
		struct MaterialData
		{
			Material material;
			std::string diffuseMapName;
			std::unique_ptr<Texture> diffuseMap;
		};

		std::vector<MeshData> meshData;
		std::vector<MaterialData> materialData;
		Skeleton skeleton;

		std::vector<Math::Matrix4> boneMatrices;
		AnimationSet animationSet;
	};
}