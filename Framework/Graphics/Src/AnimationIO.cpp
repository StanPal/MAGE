#include "Precompiled.h"
#include "AnimationIO.h"

using namespace MAGE;
using namespace MAGE::Graphics;

void AnimationIO::Write(FILE* file, const Animation& animation)
{
	
	fprintf_s(file, "PositionSize: %d\n", static_cast<uint32_t>(animation.mPositionKeys.size()));
	for (uint32_t i = 0; i < animation.mPositionKeys.size(); ++i)
	{
		fprintf_s(file, "%f %f %f %f\n",
			animation.mPositionKeys[i].key.x,
			animation.mPositionKeys[i].key.y,
			animation.mPositionKeys[i].key.z,
			animation.mPositionKeys[i].time);
	}

	fprintf_s(file, "RotationSize: %d\n", static_cast<uint32_t>(animation.mRotationKeys.size()));
	for (uint32_t i = 0; i < animation.mRotationKeys.size(); ++i)
	{
		fprintf_s(file, "%f %f %f %f %f\n",
			animation.mRotationKeys[i].key.x,
			animation.mRotationKeys[i].key.y,
			animation.mRotationKeys[i].key.z,
			animation.mRotationKeys[i].key.w,
			animation.mRotationKeys[i].time);
	}

	fprintf_s(file, "ScaleSize: %d\n", static_cast<uint32_t>(animation.mScaleKeys.size()));
	for (uint32_t i = 0; i < animation.mScaleKeys.size(); ++i)
	{
		fprintf_s(file, "%f %f %f %f\n",
			animation.mScaleKeys[i].key.x,
			animation.mScaleKeys[i].key.y,
			animation.mScaleKeys[i].key.z,
			animation.mScaleKeys[i].time);
	}
}

void AnimationIO::Read(FILE* file, Animation& animation)
{
	uint32_t positionSize = 0;
	fscanf_s(file, "PositionSize: %d\n", &positionSize);
	animation.mPositionKeys.resize(positionSize);
	for (uint32_t i = 0; i < positionSize; ++i)
	{
		fscanf_s(file, "%f %f %f %f\n",
			&animation.mPositionKeys[i].key.x,
			&animation.mPositionKeys[i].key.y,
			&animation.mPositionKeys[i].key.z,
			&animation.mPositionKeys[i].time);
	}

	uint32_t rotationSize = 0;
	fscanf_s(file, "RotationSize: %d\n", &rotationSize);
	animation.mRotationKeys.resize(rotationSize);
	for (uint32_t i = 0; i < rotationSize; ++i)
	{
		fscanf_s(file, "%f %f %f %f %f\n",
			&animation.mRotationKeys[i].key.x,
			&animation.mRotationKeys[i].key.y,
			&animation.mRotationKeys[i].key.z,
			&animation.mRotationKeys[i].key.w,
			&animation.mRotationKeys[i].time);
	}

	uint32_t scaleSize = 0;
	fscanf_s(file, "ScaleSize: %d\n", &scaleSize);
	animation.mScaleKeys.resize(scaleSize);
	for (uint32_t i = 0; i < scaleSize; ++i)
	{
		fscanf_s(file, "%f %f %f %f\n",
			&animation.mScaleKeys[i].key.x,
			&animation.mScaleKeys[i].key.y,
			&animation.mScaleKeys[i].key.z,
			&animation.mScaleKeys[i].time);
	}
	
}

void AnimationIO::Write(FILE* file, const AnimationClip& animationClip)
{
	fprintf_s(file, "Name: %s\n", animationClip.name.c_str());
	fprintf_s(file, "Duration: %f\n", animationClip.duration);
	fprintf_s(file, "TicksPerSecond: %f\n", animationClip.ticksPerSecond);
	
	fprintf_s(file, "BoneAnimationSize: %d\n", static_cast<uint32_t>(animationClip.boneAnimations.size()));

	for (auto& bone : animationClip.boneAnimations)
	{
		//When you read in check to see if the bones have an animation or not. 
		if (bone == nullptr)
		{
			fprintf(file, "[Empty]\n");
		}
		else
		{
			fprintf(file, "Animation:\n");
			Write(file, *bone.get());
		}
	}

	//Homework
	//Note that animationClip has the same number of animation
	//as there are bones. This is so that we can use the bone index
	//to index for an animation directly. If animationClip.boneAnimation[i]
	//is null, you will still need to write out something like [Empty],
	//so when youi load the animation clip, you know to skip those slots

	//boneAnimations:
	//[--->][null][--->][--->][--->][null]
	//
	//output:
	//Animation: ...
	//[Empty]
	//Animation: ...
	//Animation: ...
	//Animation: ...
	//[Empty]

}

void AnimationIO::Read(FILE* file, AnimationClip& animationClip)
{
	char name[64];
	fscanf_s(file, "Name: %s\n", &name, sizeof(name));
	animationClip.name = name;
	fscanf_s(file, "Duration: %f\n", &animationClip.duration);
	fscanf_s(file, "TicksPerSecond: %f\n", &animationClip.ticksPerSecond);
	uint32_t animationSize = 0;
	fscanf_s(file, "BoneAnimationSize: %d\n", &animationSize);
	animationClip.boneAnimations.resize(animationSize);

	for (auto& bone : animationClip.boneAnimations)
	{		
		fscanf_s(file, "%s\n", &name, sizeof(name));
		if (std::string(name) == "Animation:")
		{
			//Create the data with unique so when you read in you assign all the values. 
			bone = std::make_unique<Animation>();
			Read(file, *bone.get());
		}
	}

}
