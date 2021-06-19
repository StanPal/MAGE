#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "Component.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"

using namespace MAGE;

GameObject* GameObjectFactory::Create(GameObjectAllocator& allocator, std::filesystem::path templateFileName)
{
	using namespace rapidjson;
	auto gameObject = allocator.New();

	if (gameObject)
	{
		FILE* file = nullptr;
		fopen_s(&file, templateFileName.u8string().c_str(), "r");

		char readBuffer[65536];
		FileReadStream is(file, readBuffer, sizeof(readBuffer));

		Document document;
		document.ParseStream(is);

		if (document.HasMember("GameObject") && document["GameObject"].IsObject())
		{
			auto jsonObject = document["GameObject"].GetObjectW();
			if (jsonObject.HasMember("Components") && jsonObject["Components"].IsObject())
			{
				auto components = jsonObject["Components"].GetObjectW();
				for (auto& component : components)
				{
					auto metaClass = Core::Meta::FindMetaClass(component.name.GetString());
					auto newComponent = gameObject->AddComponent(metaClass);
					metaClass->Deserialize(newComponent, component.value);
				}
			}
		}
		fclose(file);
	}
	return gameObject;
	//if (document.HasMember("GameObject"))
	//{
	//	auto gameobject = document["GameObject"].GetObjectW();
	//	if (gameobject.HasMember("Components"))
	//	{
	//		auto components = gameobject["Components"].GetObjectW();
	//		if (components.HasMember("TransformComponent"))
	//		{
	//			auto getTransform = components["TransformComponent"].GetObjectW();
	//			if (getTransform.HasMember("Position"))
	//			{
	//				auto Position = getTransform["Position"].GetObjectW();
	//				auto Transform = newObject->AddComponent<TransformComponent>();
	//				
	//				assert(Position["x"].IsNumber());
	//				assert(Position["x"].IsFloat());

	//				Transform->mPosition.x = Position["x"].GetFloat();
	//				Transform->mPosition.y = Position["y"].GetFloat();
	//				Transform->mPosition.z = Position["z"].GetFloat();
	//			}				
	//		}
	//		if (components.HasMember("ColliderComponent"))
	//		{
	//			auto getCollider = components["ColliderComponent"].GetObjectW();
	//			auto collider = newObject->AddComponent<ColliderComponent>();
	//			if (getCollider.HasMember("Center"))
	//			{
	//				auto Center = getCollider["Center"].GetObjectW();
	//				collider->mCenter.x = Center["x"].GetFloat();
	//				collider->mCenter.y = Center["y"].GetFloat();
	//				collider->mCenter.z = Center["z"].GetFloat();
	//			}
	//			if (getCollider.HasMember("Extend"))
	//			{
	//				auto Extend = getCollider["Extend"].GetObjectW();
	//				collider->mExtend.x = Extend["x"].GetFloat();
	//				collider->mExtend.y = Extend["y"].GetFloat();
	//				collider->mExtend.z = Extend["z"].GetFloat();
	//			}
	//		}
	//	}
	//}	
	
}


void GameObjectFactory::Destroy(GameObjectAllocator& allocator, GameObject* gameObject)
{
	allocator.Delete(gameObject);
}