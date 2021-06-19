#pragma once

#include "Colors.h"

namespace MAGE::Graphics
{
	struct Material
	{
		Color ambient;
		Color diffuse;
		Color specular;
		float power;
		float padding[3];
	};
}
