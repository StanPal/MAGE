#include "GameState.h"

using namespace MAGE::Graphics;
using namespace MAGE::Input;
using namespace MAGE::Math;
using namespace MAGE::Physics;

//ToDO: Move this function into a new file/name: AnimationUtil and add it to the Graphics Library
namespace
{
	//Error threshold is how close do we need to get, if we 
	//are close enough we are done
	void RunFABRIK(std::vector<Vector3>& points, const Vector3& target,int maxIteration ,float errorThreshhold)
	{
		ASSERT(points.size() >= 2, "Not enough points to run FABRIK!");

		// Record the origin
		Vector3 origin = points.front();


		// Record the current point distances, we want to maintain these values when running IK
		//(IK = Inverse Kinematics)
		std::vector<float> lengths;
		lengths.reserve(points.size() - 1);

		for (size_t i = 0; i + 1 < points.size(); ++i)
			lengths.push_back(Magnitude(points[i + 1] - points[i]));

		// Loop until we are close enough, or have used up all iteration
		for (int i = 0; i < maxIteration; ++i)
		{
			if (MagnitudeSqrd(points.back() - target) < errorThreshhold * errorThreshhold)
				break;
			// Run forward pass
			points.back() = target;
			for (int i = (int)points.size() - 2; i >= 0; --i)
			{
				Vector3 direction = Normalize(points[i + 1] - points[i]);
				points[i] = points[i + 1] - (direction * lengths[i]);
			}
			//Run backward pass
			points.front() = origin;
			for (int i = 0; i + 1 < (int)points.size(); ++i)
			{
				Vector3 direction = Normalize(points[i + 1] - points[i]);
				points[i + 1] = points[i] + (direction * lengths[i]);
			}
		}
	}
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	
	mCamera.SetNearPlane(0.1f);
	mCamera.SetFarPlane(300.0f);
	mCamera.SetPosition({ 0.0f, 3.0f, -10.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	const int numBones = 4;
	const float boneLength = 1.0f; 

	mPoints.reserve(numBones + 1);
	mPoints.push_back(Vector3::Zero);
	for (size_t i = 1; i < mPoints.capacity(); ++i)
	{
		mPoints.push_back(mPoints.back() + Vector3{ 0.0f, boneLength, 0.0f });
	}
	mTarget = mPoints.back();
}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{
	 float kMoveSpeed = 10.0f;
	const float kTurnSpeed = 1.0f;

	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::LSHIFT))
		kMoveSpeed = 20.0f;

	//Camera Control
	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Strafe(-kMoveSpeed * deltaTime);


	//Target Control
	if (inputSystem->IsKeyDown(KeyCode::UP))
		mTarget.y = mTarget.y + kMoveSpeed * deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::DOWN))
		mTarget.y = mTarget.y - kMoveSpeed * deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
		mTarget.x = mTarget.x + kMoveSpeed * deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::LEFT))
		mTarget.x = mTarget.x - kMoveSpeed * deltaTime;


	
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
		mCamera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
		mCamera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);

	RunFABRIK(mPoints, mTarget, 5, 0.01f);

}

void GameState::Render()
{
	for (size_t i = 0; i + 1 < mPoints.size(); ++i)
	{
		SimpleDraw::AddLine(mPoints[i], mPoints[i + 1], Colors::Green);		
		SimpleDraw::AddSphere(mPoints[i], 0.1f, Colors::Cyan,4,8);
	}
	SimpleDraw::AddAABB(mTarget, { 0.2f, 0.2f, 0.2f }, Colors::Red);

	SimpleDraw::AddGroundPlane(30.0f);
	SimpleDraw::AddTransform(Matrix4::Identity);
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	
}

