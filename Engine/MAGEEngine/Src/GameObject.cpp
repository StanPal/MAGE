#include "Precompiled.h"
#include "GameObject.h"

#include "Component.h"

using namespace MAGE;

META_CLASS_BEGIN(GameObject)
	META_NO_FIELD
META_CLASS_END

void GameObject::Initialize()
{
	ASSERT(!mInitialized, "GameObject -- Game object already initalized!");
	for (auto& component : mComponents)
	{
		component->Initialize();
	}
	mInitialized = true;
}

void GameObject::Terminate()
{
	for (auto& component : mComponents)
	{
		component->Terminate();
	}
}

void GameObject::Update(float deltaTime)
{
	for (auto& component : mComponents)
	{
		component->Update(deltaTime);
	}
}

void GameObject::Render()
{
	for (auto& component : mComponents)
	{
		component->Render();
	}
}

void GameObject::DebugUI()
{
	for (auto& component : mComponents)
	{
		component->DebugUI();
	}
}

Component* GameObject::AddComponent(const Core::Meta::MetaClass* metaClass)
{
	Component* newComponent = static_cast<Component*>(metaClass->Create());
	newComponent->mOwner = this;
	mComponents.emplace_back(std::unique_ptr<Component>(newComponent));
	return newComponent;
}
