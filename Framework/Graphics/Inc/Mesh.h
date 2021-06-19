#pragma once
#include "VertexTypes.h"

namespace MAGE::Graphics
{
	template <class VertexTypes>
	struct MeshBase
	{
		std::vector<VertexTypes> mVertices;
		std::vector<uint32_t> mIndices;
	};

	using MeshPC = MeshBase<VertexPC>;
	using MeshPX = MeshBase<VertexPX>;
	using MeshPN = MeshBase<VertexPN>;
	using Mesh = MeshBase<Vertex>;
	using SkinnedMesh = MeshBase<BoneVertex>;
	
} // namespace MAGE::Graphics