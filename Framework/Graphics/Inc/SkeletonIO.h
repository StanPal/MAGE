#pragma once

#include "Skeleton.h"
namespace MAGE::Graphics::SkeletonIO
{
	void Write(FILE* file, const Skeleton& skeleton);
	void Read(FILE* file, Skeleton& Skeleton);

}
