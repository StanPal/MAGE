#pragma once
#include "Mesh.h"
namespace MAGE::Graphics
{
	class OBJLoader
	{
	public:
		static Mesh Load(const std::filesystem::path& fileName, float scale);
	};
}
