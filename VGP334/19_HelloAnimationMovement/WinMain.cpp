#include "GameState.h"
#include <MAGEEngine/Inc/MAGE.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	MAGE::MainApp().AddState<GameState>("GameState");
	MAGE::MainApp().Run({ "Hello SkeletalAnimation", 1280,720 });
	return 0;
}