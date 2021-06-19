#include "Precompiled.h"
#include "Skeleton.h"

using namespace MAGE;
using namespace MAGE::Graphics;

void MAGE::Graphics::DrawSkeleton(Bone* skeleton, const std::vector<Math::Matrix4>& boneMatrices, const Math::Vector3& modelPos)
{
	//b4 : b4.toParent * bones[b4.parentIndex].toParent * ..... 

	//The matrix in the skeleton is the matrix relevant to the parent, the Skeleton doesn't store the 
	//full matrix, it only stores the matrix how one bone goes to the next bone. 
	//Use skeleton so you know what the parent child order is
	//But, use boneMatrices(which is the multiplied out matrices) to get the position
	//Draw Lines to connect the bones

	//skeleton.root->children.reserve(skeleton.bones.size());


	if (skeleton == nullptr)
	{
		return;
	}

	//skeleton.root->parent->children = skeleton.root->parent;
	// Week 5 3:15:00
	// Use Skeleton so you know what the parent child order is
	// But use BoneMatrices (which is the multiplied out matrices to get the position
	// Draw lines to connect the bones. 

	if (skeleton->parent != nullptr)
	{

		auto myMatrix = boneMatrices[skeleton->index];
		auto parentMatrix = boneMatrices[skeleton->parent->index];
	//Translate Matrix by dx, dy, dz
	//Get the position of the multiplied out matrices		
		
		auto myPosition = MAGE::Math::GetTranslation(myMatrix);
		auto parentPosition = MAGE::Math::GetTranslation(parentMatrix);
		SimpleDraw::AddLine(myPosition + modelPos, parentPosition + modelPos, Colors::Green);
		SimpleDraw::AddSphere(parentPosition + modelPos, 1.0f, Colors::Red, 4, 4);
	}
	
	for (auto& child : skeleton->children)
	{		
		DrawSkeleton(child, boneMatrices, modelPos);
	}

}

//Week 6 video 3:25:00
void MAGE::Graphics::UpdateAnimationRecursive(std::vector<Math::Matrix4>& boneMatrices, Bone* bone, AnimationClip& clip, float time)
{
	if (bone == nullptr)
		return;

	Math::Matrix4 transform = bone->toParentTransform;
	//if return false leave it as it is keep the transform as parent
	clip.GetTransform(time, bone->index, transform);

	//if it does have an animation update the bone transform matrix
	if (bone->parent)
	{
		boneMatrices[bone->index] = transform * boneMatrices[bone->parent->index];
	}
	else
	{
		clip.GetTransform(time, bone->index, transform);
	}

	for (auto& child : bone->children)
		UpdateAnimationRecursive(boneMatrices, child, clip, time);
}


//Recursion function for updating bone and pose
void MAGE::Graphics::UpdatePose(const Bone* bone, std::vector<Math::Matrix4>& boneWorldMatrices)
{
	if (bone == nullptr)
		return;

	if (bone->parent == nullptr)
		boneWorldMatrices[bone->index] = Math::Matrix4::Identity;
	else
		boneWorldMatrices[bone->index] = bone->toParentTransform * boneWorldMatrices[bone->parent->index];

	for (auto& child : bone->children)
		UpdatePose(child, boneWorldMatrices);
}


/*
Tips for getting the bone matrices:
-----------------------------------

	   R
	   |
	   V
	  [U]
   [U]   [U]
[U]         [U]

GetTransform returns
	bone->toParentTransform if we are using the bind pose (default from the skeleton)
	clip.boneAnim[bone->index] if we are using a animation pose (at time t)


std::vector<Matrix> boneMatrices;
UpdateBoneRecursive(boneMatrices, skeleton.root)
*/