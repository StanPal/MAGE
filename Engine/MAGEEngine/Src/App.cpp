#include "Precompiled.h"
#include "App.h"

#include "MetaRegistration.h"

using namespace MAGE;
using namespace MAGE::Core;
using namespace MAGE::Graphics;
using namespace MAGE::Input;

void MAGE::App::ChangeState(const std::string & name)
{
	if (auto iter = mAppStates.find(name); iter != mAppStates.end())
	{
		mNextState = iter->second.get();
	}
		
}

void MAGE::App::Run(AppConfig appConfig)
{
	LOG("App -- Running ...");

	mAppConfig = std::move(appConfig);
	
	LOG("APP -- Registering Meta types ...");
	Core::StaticMetaRegister();
	Math::StaticMetaRegister();
	MAGE::StaticMetaRegister();
	// Initialize timer assume when the engine starts running

	TimeUtil::GetTime();

	//Setup our application window 
	mWindow.Intialize(
		GetModuleHandle(NULL), 
		mAppConfig.appName.c_str(),
		mAppConfig.windowWidth, 
		mAppConfig.windowHeight);

	//Pops a window up, our job is to check the message queue

	//Initialize game systems
	//Intialize the input systems
	auto handle = mWindow.GetWindow();
	InputSystem::StaticInitialize(handle);

	//Intialize the graphics systems
	GraphicsSystem::StaticInitialize(handle,false);
	DebugUI::StaticInitialize(handle, false, true);
	SimpleDraw::StaticInitialize();

	//Initialize the starting state
	mCurrentState = mAppStates.begin()->second.get();
	mCurrentState->Initialize();

	//When this happens this is the function you want to call, which pauses the game
	//RegisterHandler(KeyCode::Escape,[This](){Quit();

	//Add a hook OnInit

	mRunning = true; 
	while (mRunning)
	{
		mWindow.ProcessMessage();
		if (!mWindow.isActive())
		{
			Quit();
			continue;
		}
		if (mNextState)
		{
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState,nullptr);
			mCurrentState->Initialize();
		}

		auto inputSystem = InputSystem::Get();
		inputSystem->Update();

		if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			Quit();
			continue;
			
		}
		
		float deltaTIme = TimeUtil::GetDeltaTime();
		//We still want this deltatime to support the concept of pause
		/*
		if(paused)
			deltaTime = 0.0f;
		*/
		mCurrentState->Update(deltaTIme);

		auto graphicsSystem = GraphicsSystem::Get();
		//Clean back buffer
		graphicsSystem->BeginRender();
		//Draw
		mCurrentState->Render();

		DebugUI::BeginRender();
		mCurrentState->DebugUI();
		DebugUI::EndRender();

		//Switch
		graphicsSystem->EndRender();
	}
	//On Cleanup
	mCurrentState->Terminate();

	//Terminate engine systems opposite order of how we initialized
	SimpleDraw::StaticTerminate();
	DebugUI::StaticTerminate(); 
	GraphicsSystem::StaticTerminate();
	InputSystem::StaticTerminate();
	//Termine windows 
	mWindow.Terminate();

}
