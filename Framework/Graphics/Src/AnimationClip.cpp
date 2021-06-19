#include "Precompiled.h"
#include "AnimationClip.h"

using namespace MAGE;
using namespace MAGE::Graphics;

//One clip shows all the matrices and transformations of each pose
bool AnimationClip::GetTransform(float time, uint32_t boneIndex, Math::Matrix4& transform) const
{

	auto& boneAnim = boneAnimations[boneIndex];
	if (boneAnim)
	{
		//This part can be different for youm just make sure it returns the transform
		transform = boneAnim->GetTransformation(time);
		return true;
	}
	return false;

}