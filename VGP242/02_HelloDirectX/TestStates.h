#pragma once

#include <MAGEEngine/Inc/MAGE.h>

class RedState : public MAGE::AppState
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;
	
};

class BlueState : public MAGE::AppState
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;

};