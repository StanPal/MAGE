#pragma once
#include "KeyFrame.h"
namespace MAGE::Graphics
{
	class Animation
	{
	public:
		Math::Matrix4 GetTransformation(float time) const;
		Math::Vector3 GetPosition(float time) const;
		Math::Quaternion GetRotation(float time) const;
		Math::Vector3 GetScale(float time) const;
		

	private:
		//Allows the use of Private members from Animation
		friend class AnimationBuilder;
		friend class AnimationIO;
		PositionKeys mPositionKeys;
		RotationKeys mRotationKeys;
		ScaleKeys mScaleKeys;
		bool mLooping = false; 
		std::vector<float> mTime;
	};

	using Animations = std::vector<std::unique_ptr<Animation>>;
}