#pragma once

#include "GameObjectFactory.h"
#include "Service.h"

namespace MAGE
{
	class GameWorld
	{
	public:
		void Initialize(size_t capacity);
		void Terminate();

		template <class ServiceType>
		ServiceType* AddService()
		{
			static_assert(std::is_base_of_v<Service, ServiceType>, "GameWorld -- 'Service Type' must be derived from type 'Service'.");
			ASSERT(!mInitialized, "GameWorld -- Cannot add new service after world has already initialized.");

			auto& newService = mServices.emplace_back(std::make_unique<ServiceType>());
			newService->mWorld = this;
			return static_cast<ServiceType*>(newService.get());
		}

		template<class ServiceType>
		const ServiceType* GetService() const
		{
			for (auto& service : mServices)
			{
				if (service->GetMetaClass() == ServiceType::StaticGetMetaClass())
				{
					return static_cast<const ServiceType*>(service.get());
				}
			}
		}


		template<class ServiceType>
		ServiceType* GetComponent()
		{
			//Cast ourselves to a const pointer
			auto constMe = static_cast<const GameWorld*>(this);
			//Will recursively call ourself and use the const version 
			//If we have a non const version use the const. 			
			return const_cast<ServiceType*>(constMe->GetService<ServiceType>());
		}

		GameObjectHandle Create(const std::filesystem::path& templateFileName, std::string name);
		GameObjectHandle Find(const std::string& name);
		void Destroy(GameObjectHandle handle);

		void Update(float deltaTime);
		void Render();
		void DebugUI();

	private:
		void DestroyInternal(GameObject* gameObject); // Destory for Realz
		void ProcessDestroyList();

		using Services = std::vector<std::unique_ptr<Service>>;
		using GameObjectList = std::vector<GameObject*>;

		Services mServices;

		std::unique_ptr<GameObjectAllocator> mGameObjectAllocator;
		std::unique_ptr<GameObjectHandlePool> mGameObjectHandlePool;

		GameObjectList mUpdateList;
		//Go through a clean up objects we need to destory
		GameObjectList mDestroyList;

		bool mInitialized = false;
		bool mUpdating = false;
	};
}
