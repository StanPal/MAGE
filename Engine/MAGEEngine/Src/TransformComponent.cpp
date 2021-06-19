#include "Precompiled.h"
#include "TransformComponent.h"

using namespace MAGE;
using namespace MAGE::Graphics;

META_DERIVED_BEGIN(TransformComponent, Component)
	META_FIELD_BEGIN
		META_FIELD(mPosition, "Position")
		META_FIELD(mRotation, "Rotation")
		META_FIELD(mScale, "Scale")
	META_FIELD_END
META_CLASS_END

void MAGE::TransformComponent::DebugUI()
{
	auto transform = GetTransform();
	SimpleDraw::AddTransform(transform);
}

Math::Matrix4 MAGE::TransformComponent::GetTransform() const
{
	return Math::Matrix4::Transform(mPosition, mRotation, mScale);
}

void TransformComponent::SetPosition(Math::Vector3 setPos)
{
	mPosition = setPos;
}