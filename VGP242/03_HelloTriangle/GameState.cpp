#include "GameState.h"

using namespace MAGE::Graphics;
using namespace MAGE::Input;
using namespace MAGE::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::LightGray);

	mVertices.emplace_back(Vertex{ Vector3{0.0f, 0.5f , 0.0f }});
	mVertices.emplace_back(Vertex{Vector3{0.5f, -0.5f , 0.0f }});
	mVertices.emplace_back(Vertex{Vector3{-0.5f, -0.5f, 0.0f }});


	auto device = GraphicsSystem::Get()->GetDevice();

	//Create Vertex Buffer -Code Up to Assert- 
	D3D11_BUFFER_DESC bufferDesc{};
	//How much Data do we need.  
	bufferDesc.ByteWidth = 3 * sizeof(Vertex);
	//Is an Enum for now set to default (where best to allocate memory) 
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//Tell DirectX what we plan to use this memory for
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//In case we want to send data from VRAM back to RAM almost never will use
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	//Pass data in
	initData.pSysMem = mVertices.data();

	//Pass in a struct, pass in the content to initialize the struct, write it to my pointer
	//HRESULT is just error code check macro as a long. 
	HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);	
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");

	//Compile and create vertex shader 
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	//ID3DBlob is just a byte array, a block of memory
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3DCompileFromFile(
		//"L" Is character encoding for WCHAR(2bytes)
		L"../../Assets/Shaders/DoNothing.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		//Look for function in the file called VS
		"VS", "vs_5_0",
		shaderFlags, 0, &shaderBlob, &errorBlob);

	if (errorBlob && errorBlob->GetBufferPointer())
	{
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	}

	hr = device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mVertexShader);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader.");

	//Define verte4x element descriptions
	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout; 
	vertexLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0 });
	//vertexLayout.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0 });

	//Create the input layout for gpu to read
	hr = device->CreateInputLayout(
		vertexLayout.data(),
		(UINT)vertexLayout.size(),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&mInputLayout
	);

	ASSERT(SUCCEEDED(hr), "Failed to create Input Layout.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);

	//Compile file for pixel shader 
	hr = D3DCompileFromFile(
		//"L" Is character encoding for WCHAR(2bytes)
		L"../../Assets/Shaders/DoNothing.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		//Look for function in the file called VS
		"PS", "ps_5_0",
		shaderFlags, 0, &shaderBlob, &errorBlob);

	if (errorBlob && errorBlob->GetBufferPointer())
	{
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	}

	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void GameState::Terminate()
{
	//Release the memory that we allocated 
	SafeRelease(mVertexBuffer);
	SafeRelease(mVertexShader);
	SafeRelease(mInputLayout);
	SafeRelease(mPixelShader);
}

void GameState::Update(float deltaTime)
{
	
}

void GameState::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();

	context->IASetInputLayout(mInputLayout);
	//In this case we have a blob and every 12 bytes is a position, so topology determines how much I grab to create that shape
	//Grabs 3 vertices and creates a triangle
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	//How big is one step, step size
	UINT stride = sizeof(Vertex);
	//We want to draw the whole triangle
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	context->VSSetShader(mVertexShader, nullptr, 0);
	context->PSSetShader(mPixelShader, nullptr, 0);

	context->Draw(mVertices.size(), 0);
}


