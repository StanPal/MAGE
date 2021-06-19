#include "Precompiled.h"
#include "SkeletonIO.h"

using namespace MAGE;
using namespace MAGE::Graphics;
using namespace MAGE::Math;

namespace
{
	void WriteMatrix(FILE* file, const Math::Matrix4& m)
	{
		fprintf_s(file, "%f %f %f %f\n", m._11, m._12, m._13, m._14);
		fprintf_s(file, "%f %f %f %f\n", m._21, m._22, m._23, m._24);
		fprintf_s(file, "%f %f %f %f\n", m._31, m._32, m._33, m._34);
		fprintf_s(file, "%f %f %f %f\n", m._41, m._42, m._43, m._44);
	}

	void ReadMatrix(FILE* file, Math::Matrix4& m)
	{
		fscanf_s(file, "%f %f %f %f\n", &m._11, &m._12, &m._13, &m._14);
		fscanf_s(file, "%f %f %f %f\n", &m._21, &m._22, &m._23, &m._24);
		fscanf_s(file, "%f %f %f %f\n", &m._31, &m._32, &m._33, &m._34);
		fscanf_s(file, "%f %f %f %f\n", &m._41, &m._42, &m._43, &m._44);
	}
}

void SkeletonIO::Write(FILE* file, const Skeleton& skeleton)
{
	fprintf(file, "BoneCount: %d\n", static_cast<uint32_t>(skeleton.bones.size()));
	for (auto& bone : skeleton.bones)
	{
		fprintf_s(file, "Parent Index: %d\n", bone->parentIndex);
		fprintf_s(file, "Name: %s\n", bone->name.c_str());
		fprintf_s(file, "Index: %d\n", bone->index);
		fprintf(file, "Child Count: %d\n", bone->children.size());
		for (auto& child : bone->children)
		{
			fprintf_s(file, "Child Indicies: %d\n", child->index);
		}

		WriteMatrix(file, bone->toParentTransform);
		WriteMatrix(file, bone->offsetTransform);
	}
}

void SkeletonIO::Read(FILE* file, Skeleton& skeleton)
{

	for (uint32_t i = 0; i < skeleton.bones.size(); ++i)
	{
		auto bone = std::make_unique<Bone>();
		char name[128];
		fscanf_s(file, "Parent Index: %d\n", &bone->parentIndex);
		fscanf_s(file, "Name: %s\n", &name, sizeof(name));
		bone->name = name;
		fscanf_s(file, "Index: %d\n", &bone->index);
		uint32_t childCount = 0;
		fscanf_s(file, "Child Count: %d\n", &childCount);
		bone->children.resize(childCount);

		for (uint32_t i = 0; i < childCount; ++i)
		{
			int boneIndex = 0;
			fscanf_s(file, "Child Indicies: %d\n", &boneIndex);
			bone->childIndices.push_back(boneIndex);
		}

		ReadMatrix(file, bone->toParentTransform);
		ReadMatrix(file, bone->offsetTransform);
		skeleton.bones[i] = std::move(bone);
	}
}


