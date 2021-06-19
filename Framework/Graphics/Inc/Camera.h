#pragma once

namespace MAGE::Graphics {

//This camera assumes YAxis as up direction you cannot
//look straight up nore straing down

	class Camera
	{
	public:
		void SetPosition(const Math::Vector3& position); 
		void SetYPosition(float posY, float heightOffset, float depthOffset);
		void SetDirection(const Math::Vector3 &direction);
		void SetLookAt(const Math::Vector3& target);

		//3 Degrees of freedom for translations
		void Rise(float distance);
		void Strafe(float distance);
		void Walk(float distance);

		//2 Degrees of freedom for rotations
		void Yaw(float radian);
		void Pitch(float radian); 

		void SetFov(float fov);
		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);
		void SetAspectRatio(float aspectRatio);

		const Math::Vector3& GetPosition() const { return mPosition; }
		const Math::Vector3& GetDirection() const { return mDirection; }

		Math::Matrix4 GetViewMatrix() const; 
		Math::Matrix4 GetPerspectiveMatrix() const; 
		Math::Matrix4 GetOrthographicMatrix(float width, float height) const;

	private:
		Math::Vector3 mPosition = Math::Vector3::Zero;
		Math::Vector3 mDirection = Math::Vector3::ZAxis;

		float mFov = 60.0f * Math::Constants::DegToRad;
		float mNearPlane = 1.0f; 
		float mFarPlane = 10000.0f;
		float mAspectRatio = 0.0f; 

	};


} //Namespace Mage::Graphics