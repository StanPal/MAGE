#include "Precompiled.h"
#include "Model.h"

//#include "AnimationIO.h"
#include "MeshIO.h"
#include "SkeletonIO.h"
using namespace MAGE;
using namespace MAGE::Graphics;

void ModelLoader::LoadModel(std::filesystem::path fileName, Model& model)
{
	fileName.replace_extension("model");

	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");

	uint32_t numMeshes = 0;
	fscanf_s(file, "MeshCount :%d\n", &numMeshes);

	model.meshData.resize(numMeshes);
	for (uint32_t i = 0; i < numMeshes; ++i)
	{
		fscanf_s(file, "MaterialIndex: %d\n", &model.meshData[i].materialIndex);
		MeshIO::Read(file, model.meshData[i].mesh);
	}

	//For homework, load model.materialData as well... 
	// if read <none> for texture name, there is no texture data


	fclose(file);

	fileName.replace_extension("materialData");

	fopen_s(&file, fileName.u8string().c_str(), "r");

	uint32_t numMaterial = 0;
	fscanf_s(file, "MaterialCount: %d\n", &numMaterial);
	model.materialData.resize(numMaterial);


	for (uint32_t i = 0; i < numMaterial; ++i)
	{
		//Ask Peter about this when passing in a string instead of a char array error
		//https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/c4473?view=vs-2019
		char DiffuseMap[128];
		fscanf_s(file, "DiffuseMapName: %s\n", &DiffuseMap, sizeof(DiffuseMap));
		model.materialData[i].diffuseMapName = DiffuseMap;
		//std::string fpath = model.materialData[i].diffuseMapName;
		MeshIO::Read(file, model.materialData[i].material);
	}

	fclose(file);

	for (auto& data : model.meshData)
		data.meshBuffer.Initialize(data.mesh);
	for (auto& data : model.materialData)
	{
		if (!data.diffuseMapName.empty())
		{
			std::string fullFileName = fileName.u8string().c_str();
			//Gets the path to the folder
			fullFileName = fullFileName.substr(0, fullFileName.rfind('/') + 1);
			data.diffuseMap = std::make_unique<Texture>();
			//Append file name to the path to the folder
			data.diffuseMap->Initialize(fullFileName + data.diffuseMapName);
		}
	}

}

void ModelLoader::LoadSkeleton(std::filesystem::path fileName, Skeleton& skeleton)
{
	fileName.replace_extension("skeleton");

	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");

	uint32_t boneCount = 0;
	fscanf_s(file, "BoneCount: %d\n", &boneCount);
	skeleton.bones.resize(boneCount);
	SkeletonIO::Read(file, skeleton);

	fclose(file);

	//Week 5 Video 3:33:00
	//Resolve Link here
	for (auto& bone : skeleton.bones)
	{
		if (bone->parentIndex != -1)
			bone->parent = skeleton.bones[bone->parentIndex].get();
		else
			skeleton.root = bone.get();

		bone->children.reserve(bone->childIndices.size());
		for (auto childIndex : bone->childIndices)
			bone->children.push_back(skeleton.bones[childIndex].get());
	}



}

void ModelLoader::LoadAnimationSet(std::filesystem::path fileName, AnimationSet& animSet)
{
	fileName.replace_extension("animset");

	FILE* file = nullptr;
	uint32_t clipCount = 0;
	fopen_s(&file, fileName.u8string().c_str(), "r");
	fscanf_s(file, "ClipCount: %d\n", &clipCount);
	animSet.clips.resize(clipCount);
	for (auto& clip : animSet.clips)
	{
		//Like using clip = new AnimationClip() and assigns the data
		//However with unique ptr you don't need to call delete as they 
		//are responsible for their own deletion
		clip = std::make_unique<AnimationClip>();
		AnimationIO::Read(file, *clip.get());
	}
	fclose(file);
}

void Model::Initialize(const std::filesystem::path& fileName)
{
	ModelLoader::LoadModel(fileName, *this);
	ModelLoader::LoadSkeleton(fileName, skeleton);
	ModelLoader::LoadAnimationSet(fileName, animationSet);
}

void Model::Terminate()
{
	for (auto& data : meshData)
		data.meshBuffer.Terminate();
	for (auto& bones : skeleton.bones)
		bones.reset();
}

void Model::Draw() const
{
	for (size_t i = 0; i < meshData.size(); ++i)
	{
		auto& data = meshData[i];
		materialData[data.materialIndex].diffuseMap->BindVS();
		materialData[data.materialIndex].diffuseMap->BindPS();
		data.meshBuffer.Draw();
	}
}


