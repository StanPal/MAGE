#include "Precompiled.h"
#include "Camera.h"
#include "GraphicsSystem.h"

using namespace MAGE; 
using namespace MAGE::Math;
using namespace MAGE::Graphics;

void MAGE::Graphics::Camera::SetPosition(const Math::Vector3& position)
{
	mPosition = position;
}

void MAGE::Graphics::Camera::SetYPosition(float PosY, float heightOffset, float depthOffset)
{
	mPosition = Vector3(mPosition.x, PosY + heightOffset, mPosition.z - depthOffset);
}

void MAGE::Graphics::Camera::SetDirection(const Math::Vector3& direction)
{
	mDirection = Math::Normalize(direction);
}

void MAGE::Graphics::Camera::SetLookAt(const Math::Vector3& target)
{
	mDirection = Math::Normalize(target - mPosition);
}

void MAGE::Graphics::Camera::Walk(float distance)
{
	mPosition += mDirection * distance;
}

void MAGE::Graphics::Camera::Strafe(float distance)
{
	const Math::Vector3 right = Math::Normalize(Math::Cross(Math::Vector3::YAxis, mDirection));
	mPosition += right * distance;
}

void MAGE::Graphics::Camera::Rise(float distance)
{
	//Using world Y
	mPosition += Math::Vector3::YAxis * distance;
}

void MAGE::Graphics::Camera::Yaw(float radian)
{
	const Math::Matrix4 matRot = Math::Matrix4::RotationY(radian);
	mDirection = Math::TransformNormal(mDirection, matRot);

}

void MAGE::Graphics::Camera::Pitch(float radian)
{
	const Math::Vector3 right = Math::Normalize(Math::Cross(Math::Vector3::YAxis, mDirection));
	const Math::Matrix4 matRot = Math::Matrix4::RotationAxis(right, radian);
	const Math::Vector3 newLook = Math::TransformNormal(mDirection, matRot);

	//Check to prevent from our new look direction fom being collinear with the Y axis
	const float dot = Math::Dot(newLook, Math::Vector3::YAxis);
	if (Math::Abs(dot) < 0.995f)
	{
		mDirection = newLook;
	}

}

void MAGE::Graphics::Camera::SetFov(float fov)
{
	constexpr float kMinFov = 10.0f * Math::Constants::DegToRad;
	constexpr float kMaxFov = 170.0f * Math::Constants::DegToRad;
	mFov = Math::Clamp(fov, kMinFov, kMaxFov);
	//mFov = fov; 
}

void MAGE::Graphics::Camera::SetNearPlane(float nearPlane)
{
	mNearPlane = nearPlane;
}

void MAGE::Graphics::Camera::SetFarPlane(float farPlane)
{
	mFarPlane = farPlane;
}

void MAGE::Graphics::Camera::SetAspectRatio(float aspectRatio)
{
	mAspectRatio = aspectRatio;
}

Math::Matrix4 MAGE::Graphics::Camera::GetViewMatrix() const
{
	const Math::Vector3 l = mDirection;
	const Math::Vector3 r = Math::Normalize(Math::Cross(Math::Vector3::YAxis,l));
	const Math::Vector3 u = Math::Normalize(Math::Cross(l, r));
	const float dx = -Math::Dot(r, mPosition);
	const float dy = -Math::Dot(u, mPosition);
	const float dz = -Math::Dot(l, mPosition);
	return
	{
		r.x,u.x,l.x,0.0f,
		r.y,u.y,l.y,0.0f,
		r.z,u.z,l.z,0.0f,
		dx,	dy,	dz,	1.0f
	};

}

Math::Matrix4 MAGE::Graphics::Camera::GetPerspectiveMatrix() const
{
	float aspectRatio = mAspectRatio;
	if (aspectRatio == 0)
	{
		const auto width = GraphicsSystem::Get()->GetBackBufferWidth();
		const auto height = GraphicsSystem::Get()->GetBackBufferHeight();
		aspectRatio = static_cast<float>(width) / height;
	}
	const float h = 1.0f / tanf(mFov * 0.5f);
	const float w = h / aspectRatio;
	const float zf = mFarPlane;
	const float zn = mNearPlane;
	const float d = zf / (zf - zn);
	return {
	w,      0.0f,   0.0f,   0.0f,
	0.0f,   h,      0.0f,   0.0f,
	0.0f,   0.0f,   d,      1.0f,
	0.0f,   0.0f,   -zn * d,    0.0f

	};
}

Math::Matrix4 Camera::GetOrthographicMatrix(float width, float height) const
	{
	const float w = width; 
	const float h = height;
	const float zf = mFarPlane; 
	const float zn = mNearPlane; 

	return Math::Matrix4
		{
			2.0f / w, 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / h, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f / (zf - zn), 0.0f,
			0.0f, 0.0f, zn / (zn - zf) , 1.0f
		}; 
}
