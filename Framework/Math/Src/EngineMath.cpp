#include "Precompiled.h"
#include "EngineMath.h"

/*
Quaternion Links used
Rotation From To:
https://forums.unrealengine.com/development-discussion/c-gameplay-programming/1482788-posting-the-source-code-for-lookrotation-for-those-who-need-it
Slerp:
https://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
https://en.wikipedia.org/wiki/Slerp
Axis-Angle Rotation:
Peters Quaternion Slide #16
Rotation Quaternion using Matrix:
Peters Quaternion Slide #18
Matrix to Quaternion:
http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
*/

using namespace MAGE::Math;

//Definition
const Vector3 Vector3::Zero{ 0.0f };
const Vector3 Vector3::One{ 1.0f };
const Vector3 Vector3::XAxis{ 1.0f,0.0f,0.0f };
const Vector3 Vector3::YAxis{ 0.0f,1.0f,0.0f };
const Vector3 Vector3::ZAxis{ 0.0f,0.0f,1.0f };
const Matrix4 Matrix4::Identity
{
	1.0f,0.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,0.0f,
	0.0f,0.0f,1.0f,0.0f,
	0.0f,0.0f,0.0f,1.0f
};
const Quaternion Quaternion::Zero{ 0.0f,0.0f,0.0f,0.0f };
const Quaternion Quaternion::Identity{ 0.0f,0.0f,0.0f,1.0f };

Matrix4 Matrix4::RotationAxis(const Vector3& axis, float rad)
{
	const Vector3 u = Normalize(axis);
	const float x = u.x;
	const float y = u.y;
	const float z = u.z;
	const float s = sinf(rad);
	const float c = cosf(rad);

	return {
		c + (x * x * (1.0f - c)),
		x * y * (1.0f - c) + z * s,
		x * z * (1.0f - c) - y * s,
		0.0f,

		x * y * (1.0f - c) - z * s,
		c + (y * y * (1.0f - c)),
		y * z * (1.0f - c) + x * s,
		0.0f,

		x * z * (1.0f - c) + y * s,
		y * z * (1.0f - c) - x * s,
		c + (z * z * (1.0f - c)),
		0.0f,

		0.0f, 0.0f, 0.0f, 1.0f
	};
}

Matrix4 Matrix4::RotationQuaternion(const Quaternion& q)
{
	return
	{
		1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z),
		(2.0f * q.x * q.y) + (2.0f * q.z * q.w),
		(2.0f * q.x * q.z) - (2.0f * q.y * q.w),
		0.0f,
		(2.0f * q.x * q.y) - (2.0f * q.w * q.z),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.z * q.z),
		(2.0f * q.y * q.z) + (2.0f * q.w * q.x),
		0.0f,
		(2.0f * q.x * q.z) + (2.0f * q.w * q.y),
		(2.0f * q.y * q.z) - (2.0f * q.w * q.x),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.y * q.y),
		0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

Matrix4 Matrix4::Transform(const Vector3& translation, const Quaternion& rotation, const Vector3& scale)
{
	Math::Matrix4 transform = Math::Matrix4::RotationQuaternion(rotation);
	transform._11 *= scale.x;
	transform._22 *= scale.y;
	transform._33 *= scale.z;
	transform._41 *= translation.x;
	transform._42 *= translation.y;
	transform._43 *= translation.z;
	return transform;	
}

Quaternion Quaternion::RotationAxis(const Vector3& axis, float rad)
{
	const float c = cos(rad * 0.5f);
	const float s = sin(rad * 0.5f);
	const Vector3 a = Normalize(axis);
	return Quaternion{ a.x * s, a.y * s, a.z * s, c };
}


//RotationLook creates a rotation using a forward and upward direction
//Ensures when rotationg there isnt a flipping problem
Quaternion Quaternion::RotationLook(const Vector3 direction, const Vector3 up)
{
	Vector3 forward = Normalize(direction);
	Vector3 normOrthg = Normalize(Cross(up, forward));
	Vector3 right = Cross(forward, normOrthg);
	float m11 = normOrthg.x;
	float m12 = normOrthg.y;
	float m13 = normOrthg.z;
	float m21 = right.x;
	float m22 = right.y;
	float m23 = right.z;
	float m31 = forward.x;
	float m32 = forward.y;
	float m33 = forward.z;
	Quaternion q0;
	float intensity = (m11 + m22) + m33;
	if (intensity > 0.0f)
	{
		const float num = sqrtf(intensity + 1.0f);
		q0.w = (num) * 0.5f;
		const float average = 0.5f / num;
		q0.x = (m23 - m32) * average;
		q0.y = (m31 - m13) * average;
		q0.z = (m12 - m21) * average;
		return q0;
	}
	if ((m11 >= m22) && (m11 >= m33))
	{
		const float num = sqrtf(((1.0f + m11) - m22) - m33);
		const float average = 0.5f / num;
		q0.x = 0.5f * num;
		q0.y = (m12 + m21) * average;
		q0.z = (m13 + m31) * average;
		q0.w = (m23 + m32) * average;
		return q0;
	}
	if (m22 > m33)
	{
		const float num = sqrtf(((1.0f + m22) - m11) - m33);
		const float average = 0.5f / num;
		q0.x = (m21 + m12) * average;
		q0.y = 0.5f * num;
		q0.z = (m32 + m23) * average;
		q0.w = (m31 - m13) * average;
		return q0;
	}
	const float num = sqrtf(((1.0f + m33) - m11) - m22);
	const float average = 0.5f / num;
	q0.x = (m31 + m13) * average;
	q0.y = (m32 + m23) * average;
	q0.z = 0.5f * num;
	q0.w = (m12 - m21) * average;
	return q0;
}


Quaternion Quaternion::RotationFromTo(Vector3 from, Vector3 to)
{
	Vector3 v0 = from;
	Vector3 v1 = to;
	v0 = Normalize(v0);
	v1 = Normalize(v1);

	const float d = Dot(v0, v1);
	//if d == 1 vectors are the same
	if (d >= 1.0f)
	{
		return Quaternion::Identity;
	}
	//Vectors are exactly opposite 
	else if (d <= -1.0f)
	{
		Vector3 axis{ 1.0f,0.0f,0.0f };
		axis = Cross(axis, v0);
		if (Magnitude(axis) == 0)
		{
			axis = { 0.0f,1.0f,0.0f };
			axis = Cross(axis, v0);
		}
		return { axis.x,axis.y,axis.z,0 };
	}

	const float s = sqrtf((1 + d) * 2);
	const float inverse = 1.0f / s;
	const Vector3 vect = Cross(v0, v1) * inverse;
	Quaternion q = { vect.x, vect.y, vect.z, s * 0.5f };
	return Normalize(q);
}

Quaternion Quaternion::RotationMatrix(const Matrix4 &m)
{
	//m._11 == [0][0] m._22 == [1][1] m._33 == [2][2]
	Matrix4 matTrans = Transpose(m);
	const float trace = matTrans._11 + matTrans._22 + matTrans._33;

	Quaternion q;
	if (trace > 0)
	{
		const float s = 0.5f / sqrtf(trace + 1.0f);
		q.w = 0.25f / s;
		q.x = (matTrans._32 - matTrans._23) * s;
		q.y = (matTrans._13 - matTrans._31) * s;
		q.z = (matTrans._21 - matTrans._12) * s;
		return q;

	}
	else
	{
		if ((matTrans._11 > matTrans._22) && (matTrans._11 > matTrans._33))
		{
			const float s = 2.0f * sqrtf(1.0f + matTrans._11 - matTrans._22 - matTrans._33);
			q.w = (matTrans._32 - matTrans._23) / s;
			q.x = 0.25f * s;
			q.y = (matTrans._12 + matTrans._21) / s;
			q.z = (matTrans._13 + matTrans._31) / s;
			return q;
		}
		else if (matTrans._22 > matTrans._33)
		{
			const float s = 2.0f * sqrtf(1.0f + matTrans._22 - matTrans._11 - matTrans._33);
			q.w = (matTrans._13 - matTrans._31) / s;
			q.x = (matTrans._12 + matTrans._21) / s;
			q.y = 0.25f * s;
			q.z = (matTrans._23 + matTrans._32) / s;
			return q;
		}
		else
		{
			const float s = 2.0f * sqrtf(1.0f + matTrans._33 - matTrans._11 - matTrans._22);
			q.w = (matTrans._21 - matTrans._12) / s;
			q.x = (matTrans._13 + matTrans._31) / s;
			q.y = (matTrans._23 + matTrans._32) / s;
			q.z = 0.25f * s;
			return q;
		}
	}

}

//Interview question how to tell if a point within an aabb
bool MAGE::Math::isContained(const Vector3& point, const AABB& aabb)
{
	auto min = aabb.Min();
	auto max = aabb.Max();
	if (point.x < min.x || point.x > max.x ||
		point.y < min.y || point.y > max.y || 
		point.z < min.z || point.z > max.z )
		return false;
	return true;
}
bool MAGE::Math::isContained(const Vector3& point, const OBB& obb)
{
	//Compute the world-to-local matrices
	Math::Matrix4 matTrans = Math::Matrix4::Translation(obb.center);
	Math::Matrix4 matRot = Math::Matrix4::RotationQuaternion(obb.rot);
	Math::Matrix4 matScale = Math::Matrix4::Scaling(obb.extend);
	Math::Matrix4 toWorld = matScale * matRot * matTrans;
	Math::Matrix4 toLocal = Inverse(toWorld);

	//Transform the point into the OBB's local space
	Vector3 lcoalPoint = TransformCoord(point, toLocal);

	//Test against lcoal AABB
	return isContained(lcoalPoint, AABB{ Vector3::Zero, Vector3::One });
}


bool MAGE::Math::Intersect(const Ray& ray, const Plane& plane, float& distance)
{
	const float orgDotN = Dot(ray.origin, plane.n);
	const float dirDotN = Dot(ray.direction, plane.n);

	// Check if ray is parallel to the plane
	if (Abs(dirDotN) < 0.0001f)
	{
		if (Abs(orgDotN - plane.d) < 0.0001f)
		{
			distance = 0.0f;
			return true;
		}
		else
		{
			return false;
		}
	}

	// Compute distance
	distance = (plane.d - orgDotN) / dirDotN;
	return true;
}




bool MAGE::Math::GetContactPoint(const Ray & ray, const OBB & obb, Vector3 point, Vector3 & normal)
{
	// Compute the local-to-world/world-to-local matrices
	Matrix4 matTrans = Matrix4::Translation(obb.center);
	Matrix4 matRot = Matrix4::RotationQuaternion(obb.rot);
	Matrix4 matWorld = matRot * matTrans;
	Matrix4 matWorldInv = Inverse(matWorld);

	// Transform the ray into the OBB's local space
	Vector3 org = TransformCoord(ray.origin, matWorldInv);
	Vector3 dir = TransformNormal(ray.direction, matWorldInv);
	Ray localRay{ org, dir };

	Plane planes[] =
	{
		{ {  0.0f,  0.0f, -1.0f }, obb.extend.z },
		{ {  0.0f,  0.0f,  1.0f }, obb.extend.z },
		{ {  0.0f, -1.0f,  0.0f }, obb.extend.y },
		{ {  0.0f,  1.0f,  0.0f }, obb.extend.y },
		{ { -1.0f,  0.0f,  0.0f }, obb.extend.x },
		{ {  1.0f,  0.0f,  0.0f }, obb.extend.x }
	};

	uint32_t numIntersections = 0;
	for (uint32_t i = 0; i < 6; ++i)
	{
		const float d = Dot(org, planes[i].n);
		if (d > planes[i].d)
		{
			float distance = 0.0f;
			if (Intersect(localRay, planes[i], distance) && distance >= 0.0f)
			{
				Vector3 pt = org + (dir * distance);
				if (abs(pt.x) <= obb.extend.x + 0.01f &&
					abs(pt.y) <= obb.extend.y + 0.01f &&
					abs(pt.z) <= obb.extend.z + 0.01f)
				{
					point = pt;
					normal = planes[i].n;
					++numIntersections;
				}
			}
		}
	}

	if (numIntersections == 0)
	{
		return false;
	}

	point = TransformCoord(point, matWorld);
	normal = TransformNormal(normal, matWorld);
	return true;
}






