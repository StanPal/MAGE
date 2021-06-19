#pragma once

#include "Bone.h"
#include "AnimationClip.h"
#include "SimpleDraw.h"

namespace MAGE::Graphics
{
	struct Skeleton
	{
		Bone* root = nullptr;
		std::vector<std::unique_ptr<Bone>> bones;
	};

	//void DrawSkeleton(const Skeleton& skeleton, const std::vector<Math::Matrix4>& boneMatrices);
	void DrawSkeleton(Bone* skeleton, const std::vector<Math::Matrix4>& boneMatrices, const Math::Vector3& modelPos);
	void UpdatePose(const Bone* bone, std::vector<Math::Matrix4>& boneWorldMatrices);
	void UpdateAnimationRecursive(std::vector<Math::Matrix4>& boneMatrices, Bone* bone, AnimationClip& clip, float time);

	void SetSkeletonPosition(const Math::Vector3& pos);
}


