#pragma once

namespace MAGE::Math {

	struct Vector4
	{
		float x, y, z, w;

		//Nested Constructors
		constexpr Vector4() noexcept : Vector4{ 0.0f } {}
		constexpr Vector4(float f) noexcept : Vector4(f, f, f,f) {}
		constexpr Vector4(float x, float y, float z, float w) noexcept : x{ x }, y{ y }, z{ z }, w{ w }{}

		//Delcaration
		const static Vector4 Zero;
		const static Vector4 One;
		const static Vector4 XAxis;
		const static Vector4 YAxis;
		const static Vector4 ZAxis;
		const static Vector4 WAxis;

		constexpr Vector4 operator- (const Vector4& obj) { return { x - obj.x,y - obj.y,z - obj.z, w }; }
		constexpr Vector4 operator+ (const Vector4& obj) { return { x + obj.x , y + obj.y , z + obj.z, w  }; }
		constexpr Vector4 operator* (const Vector4& obj) { return { x * obj.x, y * obj.y , z * obj.z, w  }; }
		constexpr Vector4 operator/ (const Vector4& obj) { return { x / obj.x, y / obj.y, z / obj.z, w }; }

		//Homeword: add +,-,*, and /

		Vector4& operator+=(const Vector4& v) { x += v.x; y += v.y; z += v.z; w; return *this; }
		Vector4& operator-=(const Vector4& v) { x -= v.x; y -= v.y; z -= v.z; w; return *this; }
		Vector4& operator*=(const Vector4& v) { x *= v.x; y *= v.y; z *= v.z; w; return *this; }
		Vector4& operator/=(const Vector4& v) { x /= v.x; y /= v.y; z /= v.z; w; return *this; }
		// Homework: add -=, *=, /=


	};
} //Namespace MAGE::Math