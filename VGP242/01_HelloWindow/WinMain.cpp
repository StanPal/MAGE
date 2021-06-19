#include <MAGEEngine/Inc/MAGE.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	MAGE::MainApp().AddState<MAGE::AppState>("DummyState");
	MAGE::MainApp().Run({ "Hello Window", 1280,720 });

	return 0;
}