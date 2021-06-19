#pragma once
#include "Material.h"
#include "Mesh.h"


namespace MAGE::Graphics::MeshIO
{
	void Write(FILE* file, const SkinnedMesh& mesh);
	void Write(FILE* file, const Material& mat);
	void Read(FILE* file, SkinnedMesh& mesh);
	void Read(FILE* file, Material& mesh);
}
