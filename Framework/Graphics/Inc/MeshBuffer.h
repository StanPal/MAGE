#pragma once
#include "Mesh.h"
namespace MAGE::Graphics
{
	class MeshBuffer
	{
	public:
		template <class MeshType>
		void Initialize(const MeshType& mesh, bool dynamic = false)
		{
			Initialize(
				mesh.mVertices.data(),
				static_cast<int>(mesh.mVertices.size()),
				mesh.mIndices.data(),
				static_cast<int>(mesh.mIndices.size()),
				dynamic
				);
		}
	
		template<class VertexType>
		void Initialize(const VertexType* vertices, int vertexCount, bool dynamic = false)
		{
			InitializeInternal(vertices, sizeof(VertexType), vertexCount, nullptr, 0, dynamic);
		}

		template<class VertexType>
		void Initialize(const VertexType* vertices, int vertexCount, const uint32_t* indices, int indexCount, bool dynamic = false)
		{
			InitializeInternal(vertices, sizeof(VertexType), vertexCount, indices, indexCount, dynamic);
		}

		void Terminate();

		enum class Topology 
		{
			Lines, 
			Triangles
		};
		void SetTopology(Topology topology); 
		void Update(const void* vertexData, uint32_t numVertices);

		void Draw() const;
	private:

		void InitializeInternal(const void* vertices, int vertexSize, int vertexCount, const uint32_t* indices, int indexCount, bool dynamic);
		ID3D11Buffer* mVertexBuffer = nullptr;
		ID3D11Buffer* mIndexBuffer = nullptr;
		D3D11_PRIMITIVE_TOPOLOGY mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		int mVertexSize = 0;
		int mVertexCount = 0;
		int mIndexCount = 0;
	};
}
