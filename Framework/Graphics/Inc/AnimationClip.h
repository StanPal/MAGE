#pragma once

#include "Animation.h"

namespace MAGE::Graphics
{
	class AnimationClip
	{
	public:
		bool GetTransform(float time, uint32_t boneIndex, Math::Matrix4& transform) const;

		//Attack, defend, dodge Animation movement name
		std::string name; 
		//How long the animation will last
		float duration = 0.0f;
		//Playback speed, how fast you want to go through the frame 
		float ticksPerSecond = 0.0f; 
		//A list of bone animations, look up the time and transform at
		//boneAnimation[17].transform & time 
		Animations boneAnimations; 
	};
}