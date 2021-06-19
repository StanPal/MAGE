#include "GameState.h"

using namespace MAGE::Input;
using namespace MAGE::Graphics;
using namespace MAGE::Math;
void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	mCamera.SetPosition({ 0.0f,0.0f,-5.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });


	//Create Vertices of a cube(8 Vertices) *remember points are drawn in counter clockwise 
	//x y z go from -1 -> 1 
	mMesh.mVertices.emplace_back(VertexPC{ Vector3{ -0.25f,0.25f,-0.25f }, Color{Colors::Red} });
	mMesh.mVertices.emplace_back(VertexPC{ Vector3{ 0.25f,0.25f,-0.25f }, Color{Colors::Orange} });
	mMesh.mVertices.emplace_back(VertexPC{ Vector3{ -0.25f,-0.25f,-0.25f }, Color{Colors::White} });
	mMesh.mVertices.emplace_back(VertexPC{ Vector3{ 0.25f,-0.25f,-0.25f}, Color{Colors::Red} });
	mMesh.mVertices.emplace_back(VertexPC{ Vector3{ -0.25f,0.25f,0.25f }, Color{Colors::Orange} });
	mMesh.mVertices.emplace_back(VertexPC{ Vector3{ 0.25f,0.25f,0.25f }, Color{Colors::White} });
	mMesh.mVertices.emplace_back(VertexPC{ Vector3{ -0.25f,-0.25f,0.25f }, Color{Colors::Red} });
	mMesh.mVertices.emplace_back(VertexPC{ Vector3{ 0.25f,-0.25f,0.25f }, Color{Colors::Orange} });
	VertexPC* arr = mMesh.mVertices.data();

	//A cube has 36 Indices 8x8 . 
	//Front
	mMesh.mIndices.push_back(0);
	mMesh.mIndices.push_back(1);
	mMesh.mIndices.push_back(2);
	mMesh.mIndices.push_back(1);
	mMesh.mIndices.push_back(3);
	mMesh.mIndices.push_back(2);
	//Right
	mMesh.mIndices.push_back(4);
	mMesh.mIndices.push_back(6);
	mMesh.mIndices.push_back(5);
	mMesh.mIndices.push_back(6);
	mMesh.mIndices.push_back(7);
	mMesh.mIndices.push_back(5);
	//
	mMesh.mIndices.push_back(4);
	mMesh.mIndices.push_back(5);
	mMesh.mIndices.push_back(0);
	mMesh.mIndices.push_back(0);
	mMesh.mIndices.push_back(5);
	mMesh.mIndices.push_back(1);
	//
	mMesh.mIndices.push_back(2);
	mMesh.mIndices.push_back(3);
	mMesh.mIndices.push_back(6);
	mMesh.mIndices.push_back(6);
	mMesh.mIndices.push_back(3);
	mMesh.mIndices.push_back(7);
	//top
	mMesh.mIndices.push_back(4);
	mMesh.mIndices.push_back(0);
	mMesh.mIndices.push_back(6);
	mMesh.mIndices.push_back(6);
	mMesh.mIndices.push_back(0);
	mMesh.mIndices.push_back(2);
	//bottom 
	mMesh.mIndices.push_back(1);
	mMesh.mIndices.push_back(5);
	mMesh.mIndices.push_back(3);
	mMesh.mIndices.push_back(3);
	mMesh.mIndices.push_back(5);
	mMesh.mIndices.push_back(7);

	mMeshBuffer.Initialize(mMesh);
	mVertexShader.Initialize("../../Assets/Shaders/DoTransform.fx", VertexPC::Format);
	mPixelShader.Initialize("../../Assets/Shaders/DoTransform.fx");
	mConstantBuffer.Initialize(sizeof(Matrix4));
}
void GameState::Terminate()
{
	mConstantBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
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
	}
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
	{
		mRotation -= deltaTime;
	}
}
void GameState::Render()
{
	auto matWorld = Matrix4::RotationY(mRotation);
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	mVertexShader.Bind();
	mPixelShader.Bind();
	mConstantBuffer.BindVS(0);
	//Loop that draws the cube then translates the cube by i and then draws again
	for (int i = 0; i < 10; ++i)
	{
		//Rotation is based on deltatime (?/s)
	
		auto matTranslation = Matrix4::Translation(Vector3((float)i,(float)i,(float)i));
		//Order matters when doing matrix calculations, translate matrix then multiply by world, view then projection 
		auto matWVP = Transpose(matTranslation * matWorld * matView * matProj );
	
		//mMeshBuffer.Draw();
		mConstantBuffer.Update(&matWVP);
	}
	SimpleDraw::AddLine(Vector3::Zero, Vector3::XAxis, Colors::Red);
	SimpleDraw::AddLine(Vector3::Zero, Vector3::YAxis, Colors::Green);
	SimpleDraw::AddLine(Vector3::Zero, Vector3::ZAxis, Colors::Blue);
	SimpleDraw::Render(mCamera);


}

void GameState::DebugUI()
{
	ImGui::ShowDemoWindow();
}

