#include "Precompiled.h"
#include "VertexShader.h"
#include "D3DUtil.h"
#include "VertexTypes.h"

using std::vector;
using namespace MAGE;
using namespace MAGE::Graphics;

namespace
{
	auto GetVertexLayout(uint32_t vertexFormat)
	{
		vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;

		if (vertexFormat & VE_Position)
		{
			vertexLayout.push_back({ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 });
		}
		if (vertexFormat & VE_Normal)
		{
			vertexLayout.push_back({ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 });
		}
		if (vertexFormat & VE_Tangent)
		{
			vertexLayout.push_back({ "TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 });
		}
		if (vertexFormat & VE_Color)
		{
			vertexLayout.push_back({ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 });
		}
		if (vertexFormat & VE_TexCoord)
		{
			vertexLayout.push_back({ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 });
		}
		if (vertexFormat & VE_BlendIndex)
		{
			vertexLayout.push_back({ "BLENDINDICES",0,DXGI_FORMAT_R32G32B32A32_SINT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 });
		}
		if (vertexFormat & VE_BlendWeight)
		{
			vertexLayout.push_back({ "BLENDWEIGHT",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 });
		}
		return vertexLayout;
	}
}
void VertexShader::Initialize(const std::filesystem::path& filePath, uint32_t vertexFormat)
{
	Initialize(filePath, "VS", vertexFormat);
}
void VertexShader::Initialize(const std::filesystem::path& filePath, const char* shaderName, uint32_t vertexFormat)
{
	auto device = GetDevice();
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(
		filePath.wstring().c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		shaderName, "vs_5_0",
		shaderFlags, 0, &shaderBlob, &errorBlob);
	if (errorBlob && errorBlob->GetBufferPointer())
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	ASSERT(SUCCEEDED(hr), "Failed to complie vertex buffer");
	hr = device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mVertexShader);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader");

	//Define element descriptions
	const auto vertexLayout = GetVertexLayout(vertexFormat);
	//vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	//vertexLayout.push_back({ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 });
	//vertexLayout.push_back({ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 });
	hr = device->CreateInputLayout(
		vertexLayout.data(),
		static_cast<UINT>(vertexLayout.size()),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&mInputLayout);
	ASSERT(SUCCEEDED(hr), "Failed to create input layout");
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}
void VertexShader::Terminate()
{
	SafeRelease(mVertexShader);
	SafeRelease(mInputLayout);
}
void VertexShader::Bind()
{
	auto context = GetContext();
	context->IASetInputLayout(mInputLayout);
	context->VSSetShader(mVertexShader, nullptr, 0);
}
