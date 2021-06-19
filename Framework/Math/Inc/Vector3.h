#pragma once

namespace MAGE::Math {

	struct Vector3
	{
		float x, y, z;

		//Nested Constructors
		constexpr Vector3() noexcept : Vector3{ 0.0f } {}
		constexpr Vector3(float f) noexcept : Vector3(f, f, f) {}
		constexpr Vector3(float x, float y, float z) noexcept : x{ x }, y{ y }, z{ z }{}
		
		//Delcaration
		const static Vector3 Zero;
		const static Vector3 One;
		const static Vector3 XAxis;
		const static Vector3 YAxis;
		const static Vector3 ZAxis;

		constexpr Vector3 operator- (const Vector3& obj) const { return {x - obj.x,y - obj.y,z - obj.z }; }
		constexpr Vector3 operator+ (const Vector3& obj) const { return {x + obj.x , y + obj.y , z + obj.z }; }
		constexpr Vector3 operator* (const Vector3& obj) const { return {x * obj.x, y * obj.y , z * obj.z }; }
		constexpr Vector3 operator/ (const Vector3& obj) const { return {x / obj.x, y / obj.y, z / obj.z }; }

		
		
		Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
		Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
		Vector3& operator*=(const Vector3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
		Vector3& operator/=(const Vector3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }
		

		
	};
	} //Namespace MAGE::Math