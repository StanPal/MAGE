#pragma once

namespace MAGE
{
	class GameObject;

	class Component
	{
	public:
		META_CLASS_DECLARE
		Component() = default;

		Component(const Component&) = delete;
		Component& operator=(const Component&) = delete;


		virtual ~Component() = default;

		virtual void Initialize() {}
		virtual void Terminate() {}

		virtual void Update(float deltaTime) {}
		virtual void Render() {}
		virtual void DebugUI() {}

		//If you have a component you can always grab the owner
		GameObject& GetOwner() { return *mOwner; }
		const GameObject& GetOwner() const { return *mOwner; }

	private:
		//Allow GameObject class to set these private memebers 
		friend class GameObject;
		GameObject* mOwner = nullptr;
	};
}