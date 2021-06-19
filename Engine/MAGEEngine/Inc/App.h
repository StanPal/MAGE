#pragma once
#include "AppState.h"
namespace MAGE
{
	struct AppConfig
	{
		//Default member initialization
		std::string appName = "MAGE"; 
		uint32_t windowWidth = 1280; 
		uint32_t windowHeight = 720; 
	};

	class App
	{
	public: 
		//The clause is if it works we cast it to void to 'eat it' if it doesnt work it will compain
		template <class StateType>
		//Lets you New something that doesn't know what it is in your apps
		void AddState(std::string name)
		{
			static_assert(std::is_base_of_v<AppState , StateType>, "App--Cannot add type T unless it is derived from Component.");

			mAppStates.emplace(std::move(name), std::make_unique<StateType>());
		}

		void ChangeState(const std::string& name);

		void Run(AppConfig appConfig);
		void Quit() { mRunning = false; }
	private:
		AppConfig mAppConfig; 
		Core::Window mWindow; 
		bool mRunning = false; 

		std::map<std::string, std::unique_ptr<AppState>> mAppStates;
		AppState* mCurrentState = nullptr;
		AppState* mNextState = nullptr; 
	};


}// namespace MAGE 
