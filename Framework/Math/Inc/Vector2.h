#pragma once
namespace MAGE::Math {

	struct Vector2
	{
		float x, y;
		constexpr Vector2() noexcept : Vector2{ 0.0f } {}
		constexpr Vector2(float f) noexcept : Vector2(f, f) {}
		constexpr Vector2(float x, float y) noexcept : x{ x }, y{ y }{}

		const static Vector2 Zero;
		const static Vector2 One;

		Vector2& operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
		Vector2& operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }
		Vector2& operator*=(const Vector2& v) { x *= v.x; y *= v.y; return *this; }
		Vector2& operator/=(const Vector2& v) { x /= v.x; y /= v.y; return *this; }

	};
}