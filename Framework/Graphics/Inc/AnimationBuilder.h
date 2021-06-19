#pragma once
#include "KeyFrame.h"
#include "Animation.h"
namespace MAGE::Graphics
{
	class AnimationBuilder
	{
	public:
		AnimationBuilder() = default;
		AnimationBuilder& SetLooping(bool looping);
		AnimationBuilder& SetTime(float time);
		Math::Matrix4 ReturnTransformation(float time);

		AnimationBuilder& AddPositionKey(const Math::Vector3& position);
		AnimationBuilder& AddRotationKey(const Math::Quaternion& rotation);
		AnimationBuilder& AddScaleKey(const Math::Vector3& scale);

		Animation& Get();
		AnimationBuilder& AddPositionKey(const Math::Vector3& position, float time);
		AnimationBuilder& AddRotationKey(const Math::Quaternion& rotation, float time);
		AnimationBuilder& AddScaleKey(const Math::Vector3& scale, float time);

		AnimationBuilder& Reset();
	
	private: 
		Math::Matrix4 mTransformation; 
		Animation mAnimation;
		float mTime = 0.0f; 
		bool mLooping = false; 
	};
}