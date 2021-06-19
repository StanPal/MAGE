#include "GameState.h"

using namespace MAGE::Graphics;
using namespace MAGE::Input;
using namespace MAGE::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::LightGray);
	
	InitTriforce();
	InitHeart();
	InitShip();
	auto device = GraphicsSystem::Get()->GetDevice();

	//Create Vertex Buffer -Code Up to Assert- 
	D3D11_BUFFER_DESC bufferDesc{};
	//How much Data do we need.  
	bufferDesc.ByteWidth = mTriforce.size() * sizeof(Vertex);
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
	initData.pSysMem = mTriforce.data();

	//Pass in a struct, pass in the content to initialize the struct, write it to my pointer
	//HRESULT is just error code check macro as a long. 
	HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);	
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");

	D3D11_BUFFER_DESC bufferHeartDesc{};
	//How much Data do we need.  
	bufferHeartDesc.ByteWidth = mHeart.size() * sizeof(Vertex);
	//Is an Enum for now set to default (where best to allocate memory) 
	bufferHeartDesc.Usage = D3D11_USAGE_DEFAULT;
	//Tell DirectX what we plan to use this memory for
	bufferHeartDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//In case we want to send data from VRAM back to RAM almost never will use
	bufferHeartDesc.CPUAccessFlags = 0;
	bufferHeartDesc.MiscFlags = 0;
	bufferHeartDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initHeartData{};
	//Pass data in
	initHeartData.pSysMem = mHeart.data();

	//Pass in a struct, pass in the content to initialize the struct, write it to my pointer
	//HRESULT is just error code check macro as a long. 
	HRESULT hr_heart = device->CreateBuffer(&bufferHeartDesc, &initHeartData, &mVertexHeartBuffer);
	ASSERT(SUCCEEDED(hr_heart), "Failed to create heart vertex buffer.");

	D3D11_BUFFER_DESC bufferCatDesc{};
	//How much Data do we need.  
	bufferCatDesc.ByteWidth = mShip.size() * sizeof(Vertex);
	//Is an Enum for now set to default (where best to allocate memory) 
	bufferCatDesc.Usage = D3D11_USAGE_DEFAULT;
	//Tell DirectX what we plan to use this memory for
	bufferCatDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//In case we want to send data from VRAM back to RAM almost never will use
	bufferCatDesc.CPUAccessFlags = 0;
	bufferCatDesc.MiscFlags = 0;
	bufferCatDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initCatData{};
	//Pass data in
	initCatData.pSysMem = mShip.data();

	//Pass in a struct, pass in the content to initialize the struct, write it to my pointer
	//HRESULT is just error code check macro as a long. 
	HRESULT hr_Circle = device->CreateBuffer(&bufferCatDesc, &initCatData, &mVertexShipBuffer);
	ASSERT(SUCCEEDED(hr_Circle), "Failed to create heart vertex buffer.");

	//Compile and create vertex shader 
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	//ID3DBlob is just a byte array, a block of memory
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3DCompileFromFile(
		//"L" Is character encoding for WCHAR(2bytes)
		L"../../Assets/Shaders/DoSomething.fx",
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
	vertexLayout.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0 });

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
		L"../../Assets/Shaders/DoSomething.fx",
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
	SafeRelease(mVertexHeartBuffer);
	SafeRelease(mVertexShipBuffer);
	SafeRelease(mVertexShader);
	SafeRelease(mInputLayout);
	SafeRelease(mPixelShader);
}

void GameState::Update(float deltaTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::ZERO))
	{
		mSelection = 0;
	}
	if (InputSystem::Get()->IsKeyPressed(KeyCode::ONE))
	{
		mSelection = 1;
	}
	if (InputSystem::Get()->IsKeyPressed(KeyCode::TWO))
	{
		mSelection = 2;
	}
	if (InputSystem::Get()->IsKeyPressed(KeyCode::THREE))
	{
		mSelection = 3;
	}

}

void GameState::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();

	context->IASetInputLayout(mInputLayout);

	
	//D3D11_PRIMITIVE_TOPOLOGY_LINELIST
	//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST

	//How big is one step, step size
	UINT stride = sizeof(Vertex);

	//We want to draw the whole triangle
	UINT offset = 0;

	switch (mSelection)
	{
	case 1:
		GraphicsSystem::Get()->SetClearColor(Colors::LightGray);
		context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
		//In this case we have a blob and every 12 bytes is a position, so topology determines how much I grab to create that shape
		//Grabs 3 vertices and creates a triangle
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->Draw(mTriforce.size(), 0);
		break;
	case 2:
		GraphicsSystem::Get()->SetClearColor(Colors::Black);	
		context->IASetVertexBuffers(0, 1, &mVertexHeartBuffer, &stride, &offset);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		context->Draw(mHeart.size(), 0);
		break; 
	case 3:
		GraphicsSystem::Get()->SetClearColor(Colors::LightGray);
		context->IASetVertexBuffers(0, 1, &mVertexShipBuffer, &stride, &offset);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		context->Draw(mShip.size(), 0);
	default:
		break;
	}
	context->VSSetShader(mVertexShader, nullptr, 0);
	context->PSSetShader(mPixelShader, nullptr, 0);
	
}

void GameState::InitTriforce()
{
	mTriforce.emplace_back(Vertex{ Vector3{-0.5f, -0.5f , 0.0f }, Colors::Yellow });
	mTriforce.emplace_back(Vertex{ Vector3{-0.25f, 0.0f , 0.0f }, Colors::Yellow });
	mTriforce.emplace_back(Vertex{ Vector3{0.0f, -0.5f, 0.0f }, Colors::Black });

	mTriforce.emplace_back(Vertex{ Vector3{0.0f,-0.5f, 0.0f}, Colors::Black });
	mTriforce.emplace_back(Vertex{ Vector3{0.25f,0.0f, 0.0f}, Colors::Yellow });
	mTriforce.emplace_back(Vertex{ Vector3{0.5f,-0.5f, 0.0f}, Colors::Yellow });

	mTriforce.emplace_back(Vertex{ Vector3{-0.25f, 0.0f, 0.0f}, Colors::Yellow });
	mTriforce.emplace_back(Vertex{ Vector3{0.0f,  0.5f, 0.0f}, Colors::Black });
	mTriforce.emplace_back(Vertex{ Vector3{0.25f, 0.0f, 0.0f}, Colors::Yellow });
}

void GameState::InitHeart()
{

	// upper right triangle
	mHeart.emplace_back(Vertex{ Vector3{ 0.75f, 0.3f,0.0f }, Color{Colors::HotPink} });
	mHeart.emplace_back(Vertex{ Vector3{ 0.5f, 0.8f,0.0f }, Color{Colors::OrangeRed} });
	mHeart.emplace_back(Vertex{ Vector3{ 0.0f, 0.3f,0.0f }, Color{Colors::HotPink} });
	// botton triangle
	mHeart.emplace_back(Vertex{ Vector3{ 0.75f,0.3f,0.0f }, Color{Colors::HotPink} });
	mHeart.emplace_back(Vertex{ Vector3{ 0.0f,-0.75f,0.0f }, Color{Colors::OrangeRed} });
	mHeart.emplace_back(Vertex{ Vector3{ -0.75f,0.3f,0.0f }, Color{Colors::HotPink} });
// upper left triangle
	mHeart.emplace_back(Vertex{ Vector3{ -0.75f,0.3f,0.0f }, Color{Colors::HotPink} });
	mHeart.emplace_back(Vertex{ Vector3{ -0.5f,0.8f,0.0f }, Color{Colors::DarkRed} });
	mHeart.emplace_back(Vertex{ Vector3{ 0.0f,0.3f,0.0f }, Color{Colors::HotPink} });

	mHeart.emplace_back(Vertex{ Vector3{ -0.75f,0.3f,0.0f }, Color{Colors::HotPink} });
	mHeart.emplace_back(Vertex{ Vector3{ 0.0f,-0.75f,0.0f }, Color{Colors::DarkRed} });
	mHeart.emplace_back(Vertex{ Vector3{ 0.75f,0.3f,0.0f }, Color{Colors::HotPink} });


	//////Top Left
	//mHeart.emplace_back(Vertex{ Vector3{-0.5f,0.5f,0.0f}, Colors::Red });
	//mHeart.emplace_back(Vertex{ Vector3{-0.3f,0.5f,0.0f}, Colors::Red });
	////Mid
	//mHeart.emplace_back(Vertex{ Vector3{-0.3f,0.5f,0.0f}, Colors::Red });
	//mHeart.emplace_back(Vertex{ Vector3{0.0f,0.1f,0.0f}, Colors::Red });
	////TopRight
	//mHeart.emplace_back(Vertex{ Vector3{0.0f,0.1f,0.0f}, Colors::Red });
	//mHeart.emplace_back(Vertex{ Vector3{0.3f,0.5f,0.0f}, Colors::Red });
	////TopRight down
	//mHeart.emplace_back(Vertex{ Vector3{0.3f,0.5f,0.0f}, Colors::Red });
	//mHeart.emplace_back(Vertex{ Vector3{0.5f,0.5f,0.0f}, Colors::Red });
	////BottomMid
	//mHeart.emplace_back(Vertex{ Vector3{0.5f,0.5f,0.0f}, Colors::Red });
	//mHeart.emplace_back(Vertex{ Vector3{0.5f,0.0f,0.0f}, Colors::Red });
	////BottomLeft
	//mHeart.emplace_back(Vertex{ Vector3{0.5f,0.0f,0.0f}, Colors::Red });
	//mHeart.emplace_back(Vertex{ Vector3{0.0f,-0.75f,0.0f}, Colors::Red });
	////BottomLeft Up
	//mHeart.emplace_back(Vertex{ Vector3{0.0f,-0.75f,0.0f}, Colors::Red });
	//mHeart.emplace_back(Vertex{ Vector3{-0.5f,0.0f,0.0f}, Colors::Red });

	//mHeart.emplace_back(Vertex{ Vector3{-0.5f,0.0f,0.0f}, Colors::Red });
	//mHeart.emplace_back(Vertex{ Vector3{-0.5f,0.5f,0.0f}, Colors::Red });
}

void GameState::InitShip()
{
	mShip.emplace_back(Vertex{ Vector3{0.5f, -0.5f , 0.0f }, Colors::LightSlateGray });
	mShip.emplace_back(Vertex{ Vector3{0.0f, 0.0f , 0.0f }, Colors::Black });
	mShip.emplace_back(Vertex{ Vector3{0.5f, -0.5f, 0.0f }, Colors::LightSlateGray });
	
	mShip.emplace_back(Vertex{ Vector3{0.95f,0.0f, 0.0f}, Colors::Black });
	mShip.emplace_back(Vertex{ Vector3{0.0f,0.5f, 0.0f}, Colors::BlueViolet });
	mShip.emplace_back(Vertex{ Vector3{-0.95f,0.0f, 0.0f}, Colors::Black });
	
	mShip.emplace_back(Vertex{ Vector3{-0.5f, -0.5f , 0.0f }, Colors::LightSlateGray });
	mShip.emplace_back(Vertex{ Vector3{0.0f, 0.0f , 0.0f }, Colors::Black });
	mShip.emplace_back(Vertex{ Vector3{0.5f, -0.5f, 0.0f }, Colors::LightSlateGray });
	
	mShip.emplace_back(Vertex{ Vector3{0.95f,0.0f, 0.0f}, Colors::Black });
	mShip.emplace_back(Vertex{ Vector3{0.0f,0.5f, 0.0f}, Colors::BlueViolet });
	mShip.emplace_back(Vertex{ Vector3{-0.95f,0.0f, 0.0f}, Colors::Black });
	
	mShip.emplace_back(Vertex{ Vector3{-0.5f, 0.0f, 0.0f}, Colors::OrangeRed });
	mShip.emplace_back(Vertex{ Vector3{0.0f, -0.5f, 0.0f}, Colors::Black });
	mShip.emplace_back(Vertex{ Vector3{0.0f, -0.75f, 0.0f}, Colors::OrangeRed });
	
	mShip.emplace_back(Vertex{ Vector3{0.5f, 0.0f, 0.0f}, Colors::OrangeRed });
	mShip.emplace_back(Vertex{ Vector3{0.0f, -0.5f, 0.0f}, Colors::Black });
	mShip.emplace_back(Vertex{ Vector3{0.0f, -0.75f, 0.0f}, Colors::Orange });
	
	//mShip.emplace_back(Vertex{ Vector3{0.5f, 0.65f , 0.0f }, Colors::Blue });
	//mShip.emplace_back(Vertex{ Vector3{0.35f, 0.5f , 0.0f }, Colors::Black });
	//mShip.emplace_back(Vertex{ Vector3{0.5f, 0.5f, 0.0f }, Colors::LightBlue });

}


