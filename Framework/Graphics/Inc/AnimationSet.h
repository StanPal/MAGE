#pragma once

#include "AnimationIO.h"

namespace MAGE::Graphics
{
	struct AnimationSet
	{
		std::vector<std::unique_ptr<AnimationClip>> clips;
	};
}