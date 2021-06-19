#include "GameState.h"

using namespace MAGE;
using namespace MAGE::Graphics;
using namespace MAGE::Input;
using namespace MAGE::Math;
using namespace MAGE::Physics;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	

	mCameraService = mWorld.AddService<CameraService>();
	mWorld.Initialize(100);
	
	auto& camera = mCameraService->GetActiveCamera();
	camera.SetNearPlane(0.1f);
	camera.SetFarPlane(300.0f);
	camera.SetPosition({ 0.0f, 10.0f,-30.0f });
	camera.SetLookAt({ 0.0f,0.0f,0.0f });

	mWorld.Create("../../Assets/Templates/test.json", "test");
}

void GameState::Terminate()
{
	mWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
	 float kMoveSpeed = 10.0f;
	const float kTurnSpeed = 1.0f;

	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::LSHIFT))
		kMoveSpeed = 20.0f;

	auto& camera = mCameraService->GetActiveCamera();
	//Camera Control
	if (inputSystem->IsKeyDown(KeyCode::W))
		camera.Walk(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		camera.Walk(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		camera.Strafe(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		camera.Strafe(-kMoveSpeed * deltaTime);
	
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
		camera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
		camera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);

	mWorld.Update(deltaTime);
}

void GameState::Render()
{
	mWorld.Render();
	SimpleDraw::AddGroundPlane(30.0f);
	SimpleDraw::Render(mCameraService->GetActiveCamera());
}

void GameState::DebugUI()
{
	mWorld.DebugUI();
}

