#include "Precompiled.h"
#include "AnimationBuilder.h"

using namespace MAGE::Math;
using namespace MAGE::Graphics;


//In Progress... 
AnimationBuilder& MAGE::Graphics::AnimationBuilder::SetLooping(bool looping)
{
	mAnimation.mLooping = looping;
	return *this;
}

/*
Getters and Setters for setting/getting time,position,rotation,scale
Each function pushes back the specified data type and time into a templatized array
from KeyFrame.h
*/
AnimationBuilder& MAGE::Graphics::AnimationBuilder::SetTime(float time)
{
	mTime = time; 
	return *this;
}

Matrix4 MAGE::Graphics::AnimationBuilder::ReturnTransformation(float time)
{

	return mAnimation.GetTransformation(time);
}

AnimationBuilder& MAGE::Graphics::AnimationBuilder::AddPositionKey(const Math::Vector3 & position)
{
	mAnimation.mPositionKeys.push_back({ position,mTime });
	return *this;
}

AnimationBuilder& MAGE::Graphics::AnimationBuilder::AddRotationKey(const Math::Quaternion & rotation)
{
	mAnimation.mRotationKeys.push_back({ rotation,mTime });
	return *this;
}

AnimationBuilder& MAGE::Graphics::AnimationBuilder::AddScaleKey(const Math::Vector3 & scale)
{
	mAnimation.mScaleKeys.push_back({ scale,mTime });
	return *this;
}

Animation& MAGE::Graphics::AnimationBuilder::Get()
{
	mAnimation.mPositionKeys = std::move(mAnimation.mPositionKeys);
	mAnimation.mRotationKeys = std::move(mAnimation.mRotationKeys);
	mAnimation.mScaleKeys = std::move(mAnimation.mScaleKeys);

	return mAnimation;
}

AnimationBuilder & MAGE::Graphics::AnimationBuilder::AddPositionKey(const Math::Vector3 & position, float time)
{
	mAnimation.mPositionKeys.push_back({ position,time });
	return *this;
}

AnimationBuilder & MAGE::Graphics::AnimationBuilder::AddRotationKey(const Math::Quaternion & rotation, float time)
{
	mAnimation.mRotationKeys.push_back({ rotation,time });
	return *this;
}

AnimationBuilder & MAGE::Graphics::AnimationBuilder::AddScaleKey(const Math::Vector3 & scale, float time)
{
	mAnimation.mScaleKeys.push_back({ scale,time });
	return *this;
}

AnimationBuilder & MAGE::Graphics::AnimationBuilder::Reset()
{
	mAnimation.mPositionKeys.clear();
	mAnimation.mRotationKeys.clear();
	mAnimation.mScaleKeys.clear();

	return *this;
}


