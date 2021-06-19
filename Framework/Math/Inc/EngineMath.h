#pragma once

#include "Common.h"

#include "Constants.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Quaternion.h"

#include "AABB.h"
#include "OBB.h"
#include "Plane.h"
#include "Ray.h"
#include "Sphere.h"

#include "MetaRegistration.h"

namespace MAGE::Math
{
	template<class T>
	constexpr T Min(T a, T b)
	{
		return (a < b) ? a : b;
	}

	template<class T>
	constexpr T Max(T a, T b)
	{
		return (a > b) ? a : b;
	}

	//Three way comparison
	//https://en.cppreference.com/w/cpp/algorithm/clamp
	template<class T>
	constexpr T Clamp(T value, T min, T max)
	{
		return Max(min, Min(max, value));
	}
	//Linear interpolation between a and b 
	template<class T>
	constexpr T Lerp(T a, T b, float t)
	{
		return a + (b - a) * t;
	}

	constexpr float Abs(float value)
	{
		return value > 0.0f ? value : -value;
	}

	constexpr float Sqr(float value)
	{
		return value * value; 
	}

	constexpr float Dot(const Vector3& a, const Vector3& b)
	{
		return ( (a.x * b.x) + (a.y * b.y) + (a.z * b.z) );
	}

	constexpr float MagnitudeSqrd(const Vector3& v)
	{
		return ((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	}

	constexpr float MagnitudeSqrd(const Quaternion& q1)
	{
		return (q1.x * q1.x + q1.y * q1.y + q1.z * q1.z + q1.w * q1.w);
	}

	inline float Magnitude(const Vector3& v)
	{
		return sqrt(MagnitudeSqrd(v));
	}

	inline Vector3 Normalize(const Vector3& v)
	{
		return v / Magnitude(v);
	}
	constexpr Vector3 Cross(const Vector3& a, const Vector3& b)
	{
		//(uyvz-uzvy)+(uzvx-uxvz)+(uxvy-uyvx)
		return Vector3(	(a.y * b.z) - (a.z * b.y),
						(a.z * b.x) - (a.x * b.z),
						(a.x * b.y ) - (a.y * b.x));
	}

	constexpr Matrix4 Transpose(const Matrix4& m)
	{
		return {m._11,m._21,m._31,m._41,
				m._12,m._22,m._32,m._42,
				m._13,m._23,m._33,m._43,
				m._14,m._24,m._34,m._44
				};
	}

	constexpr float Determinant(const Matrix4& m)
	{
		float i = m._11 * ((m._22 * m._33) - (m._23 * m._32)); 
		float j = -m._12 * ((m._21 * m._33) - (m._23 * m._31));
		float k = m._13 * ((m._21 * m._32) - (m._22 * m._31));
		float determinant = i + j + k; 
		return determinant;
	}

	constexpr Matrix4 Inverse(const Matrix4& m)
	{
		float det = Determinant(m);
		float inverseDet = 1.0f / det;
		Matrix4 mResult{
		inverseDet*(m._22 * ((m._33 * m._44) - (m._43 * m._34)) - m._23 * ((m._32 * m._44) - (m._42 * m._34)) + m._24 * ((m._32 * m._43) - (m._42 * m._33))),
		inverseDet *-(m._12 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._32 * m._44) - (m._42 * m._34)) + m._14 * ((m._32 * m._43) - (m._42 * m._33))),
		inverseDet*(m._12 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._22 * m._44) - (m._42 * m._24)) + m._14 * ((m._22 * m._43) - (m._42 * m._23))),
		inverseDet *-(m._12 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._22 * m._34) - (m._32 * m._24)) + m._14 * ((m._22 * m._33) - (m._32 * m._23))),
		inverseDet *-(m._21 * ((m._33 * m._44) - (m._43 * m._34)) - m._31 * ((m._23 * m._44) - (m._24 * m._43)) + m._41 * ((m._23 * m._34) - (m._24 * m._33))),
		inverseDet*(m._11 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._31 * m._44) - (m._41 * m._34)) + m._14 * ((m._31 * m._43) - (m._41 * m._33))),
		inverseDet *-(m._11 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._43) - (m._41 * m._23))),
		inverseDet*(m._11 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._21 * m._34) - (m._31 * m._24)) + m._14 * ((m._21 * m._33) - (m._31 * m._23))),
		inverseDet*(m._21 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._22 * m._44) - (m._42 * m._24)) + m._41 * ((m._22 * m._34) - (m._32 * m._24))),
		inverseDet *-(m._11 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._12 * m._44) - (m._42 * m._14)) + m._41 * ((m._12 * m._34) - (m._32 * m._14))),
		inverseDet*(m._11 * ((m._22 * m._44) - (m._42 * m._24)) - m._12 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._42) - (m._41 * m._22))),
		inverseDet *-(m._11 * ((m._22 * m._34) - (m._32 * m._24)) - m._21 * ((m._12 * m._34) - (m._32 * m._14)) + m._31 * ((m._12 * m._24) - (m._22 * m._14))),
		inverseDet *-(m._21 * ((m._32 * m._43) - (m._42 * m._33)) - m._31 * ((m._22 * m._43) - (m._42 * m._23)) + m._41 * ((m._22 * m._33) - (m._32 * m._23))),
		inverseDet*(m._11 * ((m._32 * m._43) - (m._42 * m._33)) - m._12 * ((m._31 * m._43) - (m._41 * m._33)) + m._13 * ((m._31 * m._42) - (m._41 * m._32))),
		inverseDet *-(m._11 * ((m._22 * m._43) - (m._42 * m._23)) - m._12 * ((m._21 * m._43) - (m._41 * m._23)) + m._13 * ((m._21 * m._42) - (m._41 * m._22))),
		inverseDet*(m._11 * ((m._22 * m._33) - (m._32 * m._23)) - m._12 * ((m._21 * m._33) - (m._31 * m._23)) + m._13 * ((m._21 * m._32) - (m._31 * m._22))) };

		return mResult;

	}

	constexpr Vector3 GetTranslation(const Matrix4& matrix)
	{
		return Vector3(matrix._41, matrix._42, matrix._43);
	}

	constexpr Vector3 GetRight(const Matrix4& matrix)
	{
		return Vector3(matrix._11, matrix._12, matrix._13);
	}

	constexpr Vector3 GetUp(const Matrix4& matrix)
	{
		return Vector3(matrix._21, matrix._22, matrix._23);
	}

	constexpr Vector3 GetLook(const Matrix4& m)
	{
		return Vector3{ m._14,m._24, m._34 };
	}

	constexpr Vector3 GetForward(const Matrix4& matrix)
	{
		return Vector3(matrix._31, matrix._32, matrix._33);
	}

	constexpr Vector3 TransformCoord(const Vector3& v, const Matrix4& m)
	{
		Vector3 result; 
		const float w = (v.x * m._14) + (v.y * m._24) + (v.z * m._34) + (1.0f * m._44);
		result.x = ((m._11 * v.x) + (m._21 * v.y) + (m._31 * v.z) + (m._41 * 1)) / w;
		result.y = ((m._12 * v.x) + (m._22 * v.y) + (m._32 * v.z) + (m._42 * 1)) / w;
		result.z = ((m._13 * v.x) + (m._23 * v.y) + (m._33 * v.z) + (m._43 * 1)) / w;
		return result;
	}

	constexpr Vector3 TransformNormal(const Vector3& v, const Matrix4& m)
	{
		Vector3 result;
		result.x = (m._11 * v.x) + (m._21 * v.y) + (m._31 * v.z);
		result.y = (m._12 * v.x) + (m._22 * v.y) + (m._32 * v.z);
		result.z = (m._13 * v.x) + (m._23 * v.y) + (m._33 * v.z);
		return result;
	}

	inline float Magnitude(const Quaternion& q1)
	{
		return sqrtf(MagnitudeSqrd(q1));
	}

	inline Quaternion Normalize(const Quaternion& q1)
	{
		return { q1.x / Magnitude(q1), q1.y / Magnitude(q1), q1.z / Magnitude(q1), q1.w / Magnitude(q1) };
	}

	constexpr Quaternion Conjugate(const Quaternion& q1)
	{
		return { -q1.x, -q1.y, -q1.z, q1.w };
	}

	constexpr float Dot(Quaternion lhs, Quaternion rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}

	//Allows smooth linear interpolation between 2 quaternions provided that the 
	//orientations are not more than a 180 degrees apart
	inline Quaternion Slerp(Quaternion& q0, Quaternion& q1, float t)
	{
		//Peters Code
		// Find the dot product
		float dot = (q0.x * q1.x) + (q0.y * q1.y) + (q0.z * q1.z) + (q0.w * q1.w);

		// Determine the direction of the rotation.
		if (dot < 0.0f)
		{
			dot = -dot;
			q1.x = -q1.x;
			q1.y = -q1.y;
			q1.z = -q1.z;
			q1.w = -q1.w;
		}
		else if (dot > 0.999f)
		{
			return Normalize(Lerp(q0, q1, t));
		}

		float theta = acosf(dot);
		float sintheta = sinf(theta);
		float scale0 = sinf(theta * (1.0f - t)) / sintheta;
		float scale1 = sinf(theta * t) / sintheta;

		// Perform the slerp
		return Quaternion
		(
			(q0.x * scale0) + (q1.x * scale1),
			(q0.y * scale0) + (q1.y * scale1),
			(q0.z * scale0) + (q1.z * scale1),
			(q0.w * scale0) + (q1.w * scale1)
		);

	}

	
	//	const float Threshold = 0.9995f;
	//	float n1;
	//	float n2;
	//	//Angle between quaternions
	//	float angle = Dot(q0, q1);
	//	bool negative = false;

	//	// If the dot product is negative, slerp won't take
	//	// the shorter path. Note that q0 and q1 are equivalent when
	//	// the negation is applied to all four components. 
	//	//Fix by negating one of them so *-1 if their dot product is < 0
	//	if (angle < 0.0f)
	//	{
	//		negative = true;
	//		angle = -angle;
	//	}
	//	if (angle > Threshold)
	//	{
	//		n2 = 1 - t;
	//		n1 = negative ? -t : t;
	//	}
	//	else
	//	{
	//		float thetha = acosf(angle);
	//		float inverseSinThetha = 1.0f / sinf(thetha);
	//		n2 = sinf((1.0f - t) * thetha) * inverseSinThetha;
	//		if (negative)
	//		{
	//			n1 = -sinf(t * thetha) * inverseSinThetha;
	//		}
	//		else
	//		{
	//			n1 = sinf(t * thetha) * inverseSinThetha;
	//		}
	//	}
	//	Quaternion quaternion;
	//	//Lerps xyz
	//	quaternion.x = (n2 * q0.x) + (n1 * q1.x);
	//	quaternion.y = (n2 * q0.y) + (n1 * q1.y);
	//	quaternion.z = (n2 * q0.z) + (n1 * q1.z);
	//	quaternion.w = (n2 * q0.w) + (n1 * q1.w);
	//	return Normalize(quaternion);	
	//}


	bool Intersect(const Ray& ray, const Plane& plane, float& distance);

	bool isContained(const Vector3& point, const AABB& aabb);
	bool isContained(const Vector3& point, const OBB& obb);

	bool GetContactPoint(const Ray& ray, const OBB& obb, Vector3 point, Vector3& normal);



} //Namespace MAGE::Math