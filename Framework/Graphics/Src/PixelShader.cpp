#include "Precompiled.h"
#include "PixelShader.h"
#include "D3DUtil.h"

using namespace MAGE;
using namespace MAGE::Graphics;
void PixelShader::Initialize(const std::filesystem::path& filePath, const char* shaderName)
{
	auto device = GetDevice();
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(
		filePath.wstring().c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		shaderName, "ps_5_0",
		shaderFlags, 0, &shaderBlob, &errorBlob);
	if (errorBlob && errorBlob->GetBufferPointer())
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	ASSERT(SUCCEEDED(hr), "Failed to complie pixel buffer");
	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader");
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}
void PixelShader::Terminate()
{
	SafeRelease(mPixelShader);
}
void PixelShader::Bind()
{
	auto context = GetContext();
	context->PSSetShader(mPixelShader, nullptr, 0);
}
