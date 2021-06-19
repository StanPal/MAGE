#pragma once

#include "Component.h"

namespace MAGE
{
	class TransformComponent : public Component
	{
	public:

		META_CLASS_DECLARE

		void DebugUI() override;
		void SetPosition(MAGE::Math::Vector3 setPosition);

		void SetPos(Math::Vector3& position) { mPosition = position; }
		void SetScale(Math::Vector3& scale) { mScale = scale; }
		void SetQuaternion(Math::Quaternion& rotation) { mRotation = rotation; }


		Math::Vector3 GetPos() const { return mPosition; }
		Math::Vector3 GetScale() const { return mScale; }
		Math::Quaternion GetQuaternion() const { return mRotation; }
		Math::Matrix4 GetTransform() const;
		Math::Vector3 mPosition = Math::Vector3::Zero;
	private:
		Math::Quaternion mRotation = Math::Quaternion::Identity;;
		Math::Vector3 mScale = Math::Vector3::One;		
	};
}
