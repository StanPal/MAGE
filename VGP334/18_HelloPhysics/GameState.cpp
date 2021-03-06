#include "GameState.h"

using namespace MAGE::Graphics;
using namespace MAGE::Input;
using namespace MAGE::Math;
using namespace MAGE::Physics;


namespace
{
	std::random_device sRandomDevice{};
	std::mt19937 sRandomEngine{ sRandomDevice() };

	float Randf(float min, float max)
	{
		return std::uniform_real_distribution<float>{min, max}(sRandomDevice);
	}
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	
	mCamera.SetNearPlane(0.1f);
	mCamera.SetFarPlane(300.0f);
	mCamera.SetPosition({ 0.0f, 10.0f, -30.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	MAGE::Physics::PhysicsWorld::Settings settings;
	settings.gravity = { 0.0f, -9.8f, 0.0f };
	settings.timeStep = 1.0f / 60.0f;
	//Modify to increase or decrease friction
	settings.drag = 0.0f;
	settings.iterations = 1;
	mPhysicsWorld.Initialize(settings);
	mPhysicsWorld.AddStaticPlane({ Vector3::YAxis, 0.0f });
	mPhysicsWorld.AddStaticOBB({ {2.0f, 2.0f, 0.0f}, {4.0f, 0.5f, 5.0f}, Quaternion::RotationAxis(Vector3::ZAxis, 10.0f * MAGE::Math::Constants::DegToRad) });
	mPhysicsWorld.AddStaticOBB({ {-3.0f, 4.5f, 0.0f}, {4.0f, 0.2f, 5.0f}, Quaternion::RotationAxis(Vector3::ZAxis, -15.0f * MAGE::Math::Constants::DegToRad) });


}

void GameState::Terminate()
{
	mPhysicsWorld.Clear();
}

void GameState::Update(float deltaTime)
{
	 float kMoveSpeed = 10.0f;
	const float kTurnSpeed = 1.0f;

	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::LSHIFT))
		kMoveSpeed = 50.0f;

	//Left right up down
	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Strafe(-kMoveSpeed * deltaTime);

	
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
		mCamera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);	
	if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
		mCamera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
	mPhysicsWorld.Update(deltaTime);
}

void GameState::Render()
{
	mPhysicsWorld.DebugDraw();

	SimpleDraw::AddGroundPlane(30.0f);
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Physics", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::Button("Particles!"))
	{
		mPhysicsWorld.Clear(true);
		for (int i = 0; i < 100; ++i)
		{
			auto p = new Particle();
			p->SetPosition({ 0.0f,5.0f,0.0f });
			p->SetVelocity({ Randf(-0.05f,0.05f), Randf(0.1f,0.4f), Randf(-0.05f,0.05f) });
			p->radius = 0.1f;
			p->bounce = 0.7f;
			mPhysicsWorld.AddParticle(p);
		}
	}
	if (ImGui::Button("Sticks!"))
	{
		mPhysicsWorld.Clear(true);
		for (int i = 0; i < 50; ++i)
		{
			auto p1 = new Particle();
			auto p2 = new Particle();
			p1->SetPosition({ 0.5f,5.0f,0.0f });
			p1->SetVelocity({ Randf(-0.05f,0.05f), Randf(0.1f,0.4f), Randf(-0.05f,0.05f) });
			p1->radius = 0.1f;
			p1->bounce = 0.3f;

			p2->SetPosition({ -0.5f,5.0f,0.0f });
			p2->SetVelocity({ Randf(-0.05f,0.05f), Randf(0.1f,0.4f), Randf(-0.05f,0.05f) });
			p2->radius = 0.1f;
			p2->bounce = 0.3f;

			mPhysicsWorld.AddParticle(p1);
			mPhysicsWorld.AddParticle(p2);
			auto c = new Spring(p1, p2);
			mPhysicsWorld.AddConstraint(c);
		}
	}

	if (ImGui::Button("TetraHedron!"))
	{
		mPhysicsWorld.Clear(true);
		for (int i = 0; i < 20; ++i)
		{
			auto p1 = new Particle();
			auto p2 = new Particle();
			auto p3 = new Particle();
			auto p4 = new Particle();

			p1->SetPosition({ -0.5f,5.0f,0.0f });
			p1->SetVelocity({ Randf(-0.05f,0.05f), Randf(0.1f,0.4f), Randf(-0.05f,0.05f) });
			p1->radius = 0.1f;
			p1->bounce = 0.3f;

			p2->SetPosition({ 0.0f,5.0f,1.0f });
			p2->SetVelocity({ Randf(-0.05f,0.05f), Randf(0.1f,0.4f), Randf(-0.05f,0.05f) });
			p2->radius = 0.1f;
			p2->bounce = 0.3f;

			p3->SetPosition({ 0.5f,5.0f,0.0f });
			p3->SetVelocity({ Randf(-0.05f,0.05f), Randf(0.1f,0.4f), Randf(-0.05f,0.05f) });
			p3->radius = 0.1f;
			p3->bounce = 0.3f;

			p4->SetPosition({ 0.0f,6.0f,0.0f });
			p4->SetVelocity({ Randf(-0.05f,0.05f), Randf(0.1f,0.4f), Randf(-0.05f,0.05f) });
			p4->radius = 0.1f;
			p4->bounce = 0.3f;

			mPhysicsWorld.AddParticle(p1);
			mPhysicsWorld.AddParticle(p2);
			mPhysicsWorld.AddParticle(p3);
			mPhysicsWorld.AddParticle(p4);

			auto c = new Spring(p1, p2);
			mPhysicsWorld.AddConstraint(c);
		}
	}
	ImGui::End();
}

