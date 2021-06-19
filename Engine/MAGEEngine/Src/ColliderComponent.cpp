#include "Precompiled.h"
#include "ColliderComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

using namespace MAGE; 
using namespace MAGE::Graphics;


META_DERIVED_BEGIN(ColliderComponent, Component)
	META_FIELD_BEGIN		
		META_FIELD(mCenter, "Center")
		META_FIELD(mExtend, "Extend")
	META_FIELD_END
META_CLASS_END

//We want this function to be run until after there is a transform component is 
//added to the game object
void MAGE::ColliderComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
}

void MAGE::ColliderComponent::DebugUI()
{
	auto aabb = GetAABB();
	SimpleDraw::AddAABB(aabb.center,aabb.extend,Colors::LightGreen);
}

Math::AABB ColliderComponent::GetAABB() const
{
	// this is incorrect if we have orientation
	auto translation = mTransformComponent->GetPos();
	return { translation + mCenter, mExtend };
}

void ColliderComponent::Render()
{
	//SimpleDraw::AddAABB(mAABB.center, mAABB.extend, Colors::Red);
}