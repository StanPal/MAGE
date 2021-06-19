#pragma once
#include "Component.h"

namespace MAGE
{
	class TransformComponent;
	class ColliderComponent final : public Component
	{
	public:
		META_CLASS_DECLARE

		void Initialize() override;
		void DebugUI() override; 
		void Render() override;

		void SetAABB(const Math::AABB& aabb) { mAABB = aabb; }
		void SetCenter(Math::Vector3& center) { mCenter = center; }
		void SetExtend(Math::Vector3& extend) { mExtend = extend; }


		Math::Vector3 mCenter; 
		Math::Vector3 mExtend;

		Math::AABB GetAABB() const;
		Math::OBB GetOBB() const;
		Math::Sphere GetSphere() const;
		
	private:
		Math::AABB mAABB; 
		Math::Quaternion mRotation;
		//Transform component only needs to read it, to guarantee ColliderComponent
		//will not move/modify Transform Component
		const TransformComponent* mTransformComponent;
	};

}