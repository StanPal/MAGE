#include "TestStates.h"
#include <MAGEEngine/Inc/MAGE.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	MAGE::MainApp().AddState<RedState>("RedState");
	MAGE::MainApp().AddState<BlueState>("BlueState");
	MAGE::MainApp().Run({ "Hello DirectX", 1280,720 });
	return 0;
}