#include "Precompiled.h"
#include "Animator.h"

using namespace MAGE;
using namespace MAGE::Graphics;

void Animator::Initialize(const Model& model)
{
	mModel = &model;
	mBoneMatrices.resize(mModel->skeleton.bones.size());
}

void Animator::Terminate()
{
	mModel = nullptr;
}

void Animator::ComputeBindPose()
{
	UpdatePose(mModel->skeleton.root, mBoneMatrices);
}

void Animator::PlayAnimation(int index)
{
	mClipIndex = index;
	mBlendDuration = 0.0f;
}

void Animator::BlendTo(int index, float duration)
{
	mBlendWeight = 0.0f;
	mBlendTime = 0.0f;
	mBlendDuration = duration;
	mBlendIndex = index;
}

void Animator::Update(float deltaTime)
{
	auto& animationClip = mModel->animationSet.clips[mClipIndex];
	mClipDuration = animationClip->duration;
	mTimer += deltaTime * animationClip->ticksPerSecond * mSpeed;
	if (mTimer > animationClip->duration)
		mTimer -= animationClip->duration;
	UpdateAnimationRecursive(mBoneMatrices ,mModel->skeleton.root, *animationClip, mTimer);

	if (mBlendDuration > 0.0f)
	{
		auto& blendClip = mModel->animationSet.clips[mBlendIndex];
		mBlendTimer += deltaTime * blendClip->ticksPerSecond;
		if (mBlendTimer > blendClip->duration)
			mBlendTimer -= blendClip->duration;

		mBlendTime += deltaTime;
		if (mBlendTime > mBlendDuration)
		{
			mClipIndex = mBlendIndex;
			mTimer = mBlendTimer;
			mBlendWeight = 1.0f;
			mBlendDuration = 0.0f;
		}
		else
		{
			mBlendWeight = mBlendTime / mBlendDuration;
		}
		std::vector<MAGE::Math::Matrix4> targetMatrices(mBoneMatrices.size());

		UpdateAnimationRecursive(targetMatrices, mModel->skeleton.root, *blendClip, mBlendTimer);

		for (size_t i = 0; i < mBoneMatrices.size(); ++i)
		{
			mBoneMatrices[i] = mBoneMatrices[i] * (1.0f - mBlendWeight) +
				targetMatrices[i] * mBlendWeight;
		}
	}
}
