#pragma once

namespace MAGE::Math
{
	struct Matrix4;
	struct Vector3;
	struct Quaternion
	{
		//These are not the axes. 
		float x, y, z, w;

		constexpr Quaternion() noexcept : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
		constexpr Quaternion(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {}

		const static Quaternion Zero;
		const static Quaternion Identity; 

		//Create a Quaternion that represents a rotation the passed in axis and around the passed in angle  
		static Quaternion RotationAxis(const Vector3& axis, float rad);
		static Quaternion RotationMatrix(const Matrix4& m);
		static Quaternion RotationLook(Vector3 direction, Vector3 up = Vector3::YAxis);
		static Quaternion RotationFromTo(Vector3 from, Vector3 to);

		constexpr Quaternion operator- () const
		{
			return { -x, -y, -z, -w };
		}

		constexpr Quaternion operator+ (const Quaternion& rhs) const
		{
			return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
		}

		constexpr Quaternion operator- (const Quaternion& rhs) const
		{
			return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
		}

		constexpr Quaternion operator* (const float s) const
		{
			return Quaternion{ x * s, y * s, z * s, w * s };
		}

		constexpr Quaternion operator* (const Quaternion& rhs) const
		{
			float newX = (x * rhs.w) + (y * rhs.z) - (z * rhs.y) + (w * rhs.x);
			float newY = (-x * rhs.z) + (y * rhs.w) + (z * rhs.x) + (w *rhs.y);
			float newZ = (x * rhs.y) - (y * rhs.x) + (z * rhs.w) + (w * rhs.z);
			float newW = (-x * rhs.x) - (y * rhs.y) - (z * rhs.z) + (w *rhs.w);

			return { newX, newY, newZ, newW };
		}
	};

}