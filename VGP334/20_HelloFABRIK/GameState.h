#pragma once

#include <MAGEEngine\Inc\MAGE.h>

class GameState : public MAGE::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void DrawScene();
	void PostProcess();
	void DrawDepthMap();

private:
	MAGE::Graphics::Camera mCamera;
	MAGE::Physics::PhysicsWorld mPhysicsWorld;
	
	MAGE::Math::Vector3 mTarget; 
	std::vector<MAGE::Math::Vector3> mPoints; 
};