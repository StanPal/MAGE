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
	MAGE::GameWorld mWorld;
	MAGE::CameraService* mCameraService = nullptr;
	
};