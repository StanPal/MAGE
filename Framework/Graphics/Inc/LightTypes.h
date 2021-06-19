#pragma once
#include "Colors.h"

namespace MAGE::Graphics
{
	// This needs to be 16 byte alligned to be HLSL compliant 
	struct DirectionalLight
	{
		// <---		16		-->
		// [direction] [power]
		// [	ambient		]
		// [	diffuse		]
		// [	specular	]
		Math::Vector3 direction;
		float padding; 
		Color ambient;
		Color diffuse; 
		Color specular; 
	};
} 