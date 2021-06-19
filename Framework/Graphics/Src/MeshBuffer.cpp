#include "Precompiled.h"
#include "MeshBuffer.h"
#include "VertexTypes.h"
#include "Mesh.h"
#include "D3DUtil.h"

using namespace MAGE;
using namespace MAGE::Graphics;
void MeshBuffer::InitializeInternal(const void* vertices, int vertexSize,int vertexCount, const uint32_t* indices, int indexCount, bool dynamic)
{
	mVertexSize = vertexSize;
	mVertexCount = vertexCount;
	mIndexCount = indexCount;
	auto device = GetDevice();
	// Create vertex buffer
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = vertexCount * vertexSize;
	bufferDesc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0; // Use CPU
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertices;
	HRESULT hr = device->CreateBuffer(&bufferDesc, vertices ? &initData : nullptr, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer");

	if (indices != nullptr && indexCount > 0)
	{
		// Create index buffer
		bufferDesc.ByteWidth = indexCount * sizeof(uint32_t);
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		initData.pSysMem = indices;
		hr = device->CreateBuffer(&bufferDesc, &initData, &mIndexBuffer);
		ASSERT(SUCCEEDED(hr), "Failed to create index buffer");
	}
}
void MeshBuffer::Terminate()
{
	SafeRelease(mVertexBuffer);
	SafeRelease(mIndexBuffer);
}
void MAGE::Graphics::MeshBuffer::SetTopology(Topology topology)
{
	if (topology == Topology::Lines)
	{
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	}
	else
	{
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}
}
void MeshBuffer::Update(const void * vertexData, uint32_t numVertices)
{
	mVertexCount = numVertices;

	auto context = GetContext();

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, vertexData, numVertices * mVertexSize);
	context->Unmap(mVertexBuffer, 0);
}
void MeshBuffer::Draw() const
{
	auto context = GetContext();
	context->IASetPrimitiveTopology(mTopology);

	UINT stride = mVertexSize;
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	if (mIndexBuffer)
	{
		context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(mIndexCount, 0, 0);
	}
	else
	{
		context->Draw(mVertexCount, 0);
	}
}