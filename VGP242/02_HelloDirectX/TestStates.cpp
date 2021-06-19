#include "TestStates.h"

using namespace MAGE::Graphics;
using namespace MAGE::Input;

void RedState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Red);
}

void RedState::Update(float deltaTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
	{
		MAGE::MainApp().ChangeState("BlueState");
	}
}

void BlueState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Blue);
}

void BlueState::Update(float deltaTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
	{
		MAGE::MainApp().ChangeState("RedState");
	}

}
