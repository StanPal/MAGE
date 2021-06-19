#include "Precompiled.h"
#include "TimeUtil.h"

using namespace MAGE::Core;

float MAGE::Core::TimeUtil::GetTime()
{
	//HighResClock is best clock your pc gives you, which is your system clock,
	//Because it is static the first time we call it, is when you start the stopwatch
	static const auto startTime = std::chrono::high_resolution_clock::now();
	//Get the current tick
	auto now = std::chrono::high_resolution_clock::now();
	//Cast it the milliseconds when we divide it be a 1000 we get seconds
	return std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count() / 1000.0f;
}

float MAGE::Core::TimeUtil::GetDeltaTime()
{
	//Using static so that we only initialize on first call
	static auto lastTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	//Find the difference the last time you called the function to the current time you called the function
	auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f;
	lastTime = currentTime;
	return deltaTime;
}
