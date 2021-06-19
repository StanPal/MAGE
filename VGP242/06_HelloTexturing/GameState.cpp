#include "GameState.h"
using namespace MAGE::Input;
using namespace MAGE::Graphics;
using namespace MAGE::Math;
void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	mCamera.SetPosition({ 0.0f,0.0f,-5.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });

	//Homework 
	//- Update HelloTExturing to use a MeshPX data to draw a texture mapped cubes
	//- You will need to add sampler and texture classes provided
	// You will need to use DoTExturing.fx shaders 
	// Add a new class called Graphics::MeshBuilder with the following functions
	//
	//Name space MAGE::Graphics 
	// {
	//	class MeshBuilder
	//{
	//	public:
	// static MeshPX CreatePlanePX();
	// static MeshPX CreateCylinderPX();
	// static MeshPX CreateSpherePX(float radius, float rings = 16, int slices = 16);
	//}
	//
	//thiw will allow you to create a mesh easily by doing:
	//auto mesh = MeshBuilder::CreateSpherePX(...);
	// - Add HelloEarth to test a texture mapped sphere using earth texture
	//Create a sphere double for loop, for each level you step up is going from sin cosin
	/*
	A Plane:
	for(int y = 0; y < height; ++y)
	for(nt x = 0; x< width; ++x)
	vertices.push_back({x,y,0.0f}...);

	A Cylinder:
	for(int y = 0; y < height; ++y)
	for(int theata = 0; theta < twopi ; thetha += ...)
	vertices.push_back({sin(theata),y,cos(theata)}...);

	A sphere:
	for(int phi = 0; phi < Pi; phi +=...)
	for(int theata = 0; theta < twopi ; thetha += ...)
	vertices.push_back({sin(theata) * r,y,cos(theata) * r}...);

	You need 24 vertices for the textured cube
	*/

	//Create Vertices of a cube(8 Vertices) *remember points are drawn in counter clockwise 
	//x y z go from -1 -> 1 

	//Front of Cube
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{	-0.25f,	-0.25f,	-0.25f }, 0.0f,1.0f});
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{	-0.25f,	0.25f,	-0.25f },	0.0f,0.0f});
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{	0.25f,	0.25f,	-0.25f},	1.0f,0.0f});
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{	0.25f,	-0.25f,	-0.25f},	1.0f,1.0f});

	//Back of Cube
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{ -0.25f,	-0.25f,	0.25f },	1.0f,1.0f });
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{ 0.25f,	-0.25f,	0.25f },	0.0f,1.0f});
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{ 0.25f,	0.25f,	0.25f },	0.0f,0.0f});
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{ -0.25f,	0.25f,	0.25f },	1.0f,0.0f});

	//Top of Cube
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{-0.25f,	0.25f,	-0.25f},	0.0f,1.0f });
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{-0.25f,	0.25f,	0.25f},		0.0f,0.0f });
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{0.25f,	0.25f,	0.25f},		1.0f,0.0f });
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{0.25f,	0.25f,	-0.25f},	1.0f,1.0f });

	//Bottom of Cube
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{-0.25f,	-0.25f,	-0.25f},	1.0f,1.0f });
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{0.25f,	-0.25f,	-0.25f},	0.0f,1.0f });
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{0.25f,	-0.25f,	0.25f},		0.0f,0.0f });
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{-0.25f,	-0.25f,	0.25f},		1.0f,0.0f });

	//Left of Cube
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{-0.25f,	-0.25f,	0.25f},		0.0f,1.0f });
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{-0.25f,	0.25f,	0.25f},		0.0f,0.0f });
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{-0.25f,	0.25f,	-0.25f},	1.0f,0.0f });
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{-0.25f,	-0.25f,	-0.25f},	1.0f,1.0f });

	//Right of Cube
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{0.25f,	-0.25f,	-0.25f},	0.0f,1.0f });
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{0.25f,	0.25f,	-0.25f},	0.0f,0.0f });
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{0.25f,	0.25f,	0.25f},		1.0f,0.0f });
	mMesh.mVertices.emplace_back(VertexPX{ Vector3{0.25f,	-0.25f,	0.25f},		1.0f,1.0f });

	VertexPX* arr = mMesh.mVertices.data();

	//A cube has 36 Indices 8x8 . 
	//Front
	mMesh.mIndices.push_back(0);
	mMesh.mIndices.push_back(1);
	mMesh.mIndices.push_back(2);
	mMesh.mIndices.push_back(0);
	mMesh.mIndices.push_back(2);
	mMesh.mIndices.push_back(3);

	//Back
	mMesh.mIndices.push_back(4);
	mMesh.mIndices.push_back(5);
	mMesh.mIndices.push_back(6);
	mMesh.mIndices.push_back(4);
	mMesh.mIndices.push_back(6);
	mMesh.mIndices.push_back(7);

	//Top
	mMesh.mIndices.push_back(8);
	mMesh.mIndices.push_back(9);
	mMesh.mIndices.push_back(10);
	mMesh.mIndices.push_back(8);
	mMesh.mIndices.push_back(10);
	mMesh.mIndices.push_back(11);

	//Bottom
	mMesh.mIndices.push_back(12);
	mMesh.mIndices.push_back(13);
	mMesh.mIndices.push_back(14);
	mMesh.mIndices.push_back(12);
	mMesh.mIndices.push_back(14);
	mMesh.mIndices.push_back(15);

	//Left
	mMesh.mIndices.push_back(16);
	mMesh.mIndices.push_back(17);
	mMesh.mIndices.push_back(18);
	mMesh.mIndices.push_back(16);
	mMesh.mIndices.push_back(18);
	mMesh.mIndices.push_back(19);

	//Right 
	mMesh.mIndices.push_back(20);
	mMesh.mIndices.push_back(21);
	mMesh.mIndices.push_back(22);
	mMesh.mIndices.push_back(20);
	mMesh.mIndices.push_back(22);
	mMesh.mIndices.push_back(23);

	mMeshBuffer.Initialize(mMesh);
	
	mSampler.Initialize(MAGE::Graphics::Sampler::Filter::Point, MAGE::Graphics::Sampler::AddressMode::Mirror);
	mTexture.Initialize("../../Assets/Images/Rice.jpg");
	mVertexShader.Initialize("../../Assets/Shaders/DoTexturing.fx", VertexPX::Format);
	mPixelShader.Initialize("../../Assets/Shaders/DoTexturing.fx");


	mConstantBuffer.Initialize(sizeof(Matrix4));
}
void GameState::Terminate()
{
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mMeshBuffer.Terminate();
	mConstantBuffer.Terminate();
	mSampler.Terminate();
	mTexture.Terminate();
}
void GameState::Update(float deltaTime)
{
	const float kMoveSpeed = 5.5f;
	const float kTurnSpeed = 0.5f;
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(kMoveSpeed*deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-kMoveSpeed * deltaTime);
	}
	mCamera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
	mCamera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::LEFT))
	{
		mRotation += deltaTime;
		rotateUp = false;
	}
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
	{
		mRotation -= deltaTime;
		rotateUp = false;
	}

	if (inputSystem->IsKeyDown(KeyCode::UP))
	{
		mRotation += deltaTime;
		rotateUp = true;
	}
	if (inputSystem->IsKeyDown(KeyCode::DOWN))
	{
		mRotation -= deltaTime;
		rotateUp = true;
	}

	

}
void GameState::Render()
{
	Matrix4 matWorld; 
	if (!rotateUp)
	{
		matWorld = Matrix4::RotationY(mRotation);
	}
	else
	{
		matWorld = Matrix4::RotationX(mRotation);
	}

	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	mSampler.BindVS();
	mTexture.BindPS();
	mVertexShader.Bind();
	mPixelShader.Bind();
	mConstantBuffer.BindVS();
	
	auto matWVP = Transpose(matWorld * matView * matProj);

	mMeshBuffer.Draw();
	mConstantBuffer.Set(&matWVP);
	////Loop that draws the cube then translates the cube by i and then draws again
	//for (int i = 0; i < 10; ++i)
	//{
	//	//Rotation is based on deltatime (?/s)
	//
	//	auto matTranslation = Matrix4::Translation(Vector3((float)i, 0.0f, 0.0f));
	//	//Order matters when doing matrix calculations, translate matrix then multiply by world, view then projection 

	//}

}