#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

// Primitive Type Declarations
META_TYPE_DECLARE(MAGE::Math::Vector2)
META_TYPE_DECLARE(MAGE::Math::Vector3)
META_TYPE_DECLARE(MAGE::Math::Vector4)
META_TYPE_DECLARE(MAGE::Math::Quaternion)

namespace MAGE::Math
{
	void StaticMetaRegister();
}