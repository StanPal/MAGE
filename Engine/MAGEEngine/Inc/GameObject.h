#pragma once

namespace MAGE
{
	class Component;
	class GameObject;
	class GameWorld;

	using GameObjectAllocator = Core::TypedAllocator<GameObject>;
	using GameObjectHandle = Core::Handle<GameObject>;
	using GameObjectHandlePool = Core::HandlePool<GameObject>;

	class GameObject final
	{
	public:
		META_CLASS_DECLARE

		GameObject() = default;
		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;


		void Initialize();
		void Terminate();

		void Update(float deltaTime);
		void Render();
		void DebugUI();

		Component* AddComponent(const Core::Meta::MetaClass* metaClass);

		template <class ComponentType>
		ComponentType* AddComponent()
		{
			//Will be called if there is this error during build, during compile time
			static_assert(std::is_base_of_v<Component, ComponentType>, "GameObject--Cannot add type T unless it is derived from Component.");
			ASSERT(!mInitialized, "GameOject -- Cannot add new components once the game is already initialized.");

			auto& newComponent = mComponents.emplace_back(std::make_unique<ComponentType>());
			newComponent->mOwner = this;
			//Static cast is compiler verified cast
			return static_cast<ComponentType*>(newComponent.get());
		}

		template<class ComponentType>
		const ComponentType* GetComponent() const
		{
			for (auto& component : mComponents)
			{
				if (component->GetMetaClass() == ComponentType::StaticGetMetaClass())
				{
					return static_cast<const ComponentType*>(component.get());
				}
			}
			return nullptr;
		}

		template<class ComponentType>
		ComponentType* GetComponent()
		{
			//Cast ourselves to a const pointer
			auto constMe = static_cast<const GameObject*>(this);
			//Will recursively call ourself and use the const version 
			//If we have a non const version use the const. 			
			return const_cast<ComponentType*>(constMe->GetComponent<ComponentType>());
		}


		GameWorld& GetWorld() { return *mWorld; }
		const GameWorld& GetWorld() const { return *mWorld; }

		void SetName(const char* name) { mName = name; }
		const std::string& GetName() const { return mName; }

		GameObjectHandle GetHandle() const { return mHandle; }

	private:
		friend class GameWorld;
		using Components = std::vector<std::unique_ptr<Component>>;

		GameWorld* mWorld = nullptr;
		std::string mName = "NoName";
		GameObjectHandle mHandle;
		Components  mComponents;
		bool mInitialized = false;
	};
}