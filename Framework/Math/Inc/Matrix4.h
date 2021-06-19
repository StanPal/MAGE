#pragma once

namespace MAGE::Math
{
	struct Quaternion;
	struct Matrix4
	{
		union
		{
			struct
			{
				// row + column subscript
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			std::array<float, 16> v;
		};
		const static Matrix4 Identity; 

		constexpr Matrix4 operator*(const Matrix4& m) const
		{
			Matrix4 matMult{
				//Row 1
				(_11 * m._11) + (_12 * m._21) + (_13 * m._31) + (_14 * m._41),
				(_11 * m._12) + (_12 * m._22) + (_13 * m._32) + (_14 * m._42),
				(_11 * m._13) + (_12 * m._23) + (_13 * m._33) + (_14 * m._43),
				(_14 * m._14) + (_12 * m._24) + (_13 * m._34) + (_14 * m._44),

				//Row 2 									  
				(_21 * m._11) + (_22 * m._21) + (_23 * m._31) + (_24 * m._41),
				(_21 * m._12) + (_22 * m._22) + (_23 * m._32) + (_24 * m._42),
				(_21 * m._13) + (_22 * m._23) + (_23 * m._33) + (_24 * m._43),
				(_21 * m._14) + (_22 * m._24) + (_23 * m._34) + (_24 * m._44),

				//Row 3										
				(_31 * m._11) + (_32 * m._21) + (_33 * m._31) + (_34 * m._41),
				(_31 * m._12) + (_32 * m._22) + (_33 * m._32) + (_34 * m._42),
				(_31 * m._13) + (_32 * m._23) + (_33 * m._33) + (_34 * m._43),
				(_31 * m._14) + (_32 * m._24) + (_33 * m._34) + (_34 * m._44),

				//Row 4
				(_41 * m._11) + (_42 * m._21) + (_43 * m._31) + (_44 * m._41),
				(_41 * m._12) + (_42 * m._22) + (_43 * m._32) + (_44 * m._42),
				(_41 * m._13) + (_42 * m._23) + (_43 * m._33) + (_44 * m._43),
				(_41 * m._14) + (_42 * m._24) + (_43 * m._34) + (_44 * m._44)
			};
			return matMult;
		}

		constexpr Matrix4 operator*(float s) const
		{
			return
			{
			_11 * s, _12 * s, _13 * s, _14 * s,
			_21 * s, _22 * s, _23 * s, _24 * s,
			_31 * s, _32 * s, _33 * s, _34 * s,
			_41 * s, _42 * s, _43 * s, _44 * s
			};
		}

		constexpr Matrix4 operator*=(const Matrix4& m) 
		{
			Matrix4 matMult{
				//Row 1
				(_11 * m._11) + (_12 * m._21) + (_13 * m._31),
				(_11 * m._12) + (_12 * m._22) + (_13 * m._32),
				(_11 * m._13) + (_12 * m._23) + (_13 * m._33),
				//Row 2
				(_21 * m._11) + (_22 * m._21) + (_23 * m._31),
				(_21 * m._12) + (_22 * m._22) + (_23 * m._32),
				(_21 * m._13) + (_22 * m._23) + (_23 * m._33),
				//Row 3
				(_31 * m._11) + (_32 * m._21) + (_33 * m._31),
				(_31 * m._12) + (_32 * m._22) + (_33 * m._32),
				(_31 * m._13) + (_32 * m._23) + (_33 * m._33) };

			_11 = matMult._11; _12 = matMult._12; _13 = matMult._13; _14 = matMult._14;
			_21 = matMult._21; _22 = matMult._22; _13 = matMult._23; _14 = matMult._24;
			_31 = matMult._31; _32 = matMult._32; _33 = matMult._33; _34 = matMult._34;
			_41 = matMult._41; _42 = matMult._42; _43 = matMult._43; _44 = matMult._44;

			return *this;
		}

		constexpr Matrix4 operator*=(const float s)
		{
			Matrix4 matMult{
				//Row 1
				(_11 * s) + (_12 * s) + (_13 * s),
				(_11 * s) + (_12 * s) + (_13 * s),
				(_11 * s) + (_12 * s) + (_13 * s),
				(_11 * s) + (_12 * s) + (_13 * s),

				//Row 2 
				(_21 * s) + (_22 * s) + (_23 *s),
				(_21 * s) + (_22 * s) + (_23 *s),
				(_21 * s) + (_22 * s) + (_23 *s),
				(_21 * s) + (_22 * s) + (_23 *s),
				//Row 3
				(_31 * s) + (_32 * s) + (_33 * s),
				(_31 * s) + (_32 * s) + (_33 * s),
				(_31 * s) + (_32 * s) + (_33 * s),
				(_31 * s) + (_32 * s) + (_33 * s) };

			_11 = matMult._11; _12 = matMult._12; _13 = matMult._13; _14 = matMult._14;
			_21 = matMult._21; _22 = matMult._22; _13 = matMult._23; _14 = matMult._24;
			_31 = matMult._31; _32 = matMult._32; _33 = matMult._33; _34 = matMult._34;
			_41 = matMult._41; _42 = matMult._42; _43 = matMult._43; _44 = matMult._44;

			return *this;
		}

		constexpr Matrix4 operator+(const Matrix4& m) const
		{
			return
			{
			_11 + m._11, _12 + m._12, _13 + m._13, _14 + m._14,
			_21 + m._21, _22 + m._22, _23 + m._23, _24 + m._24,
			_31 + m._31, _32 + m._32, _33 + m._33, _34 + m._34,
			_41 + m._41, _42 + m._42, _43 + m._43, _44 + m._44
			};
		}

		constexpr Matrix4 operator-(const Matrix4& m) const
		{
			return
			{
			_11 - m._11, _12 - m._12, _13 - m._13, _14 - m._14,
			_21 - m._21, _22 - m._22, _23 - m._23, _24 - m._24,
			_31 - m._31, _32 - m._32, _33 - m._33, _34 - m._34,
			_41 - m._41, _42 - m._42, _43 - m._43, _44 - m._44
			};
		}

		static Matrix4 Translation(const Vector3& d)
		{
			Matrix4 matIden = Matrix4::Identity;
			matIden._41 = 1.0f * d.x; 
			matIden._42 = 1.0f * d.y;
			matIden._43 = 1.0f * d.z;
			matIden._44 = 1.0f * 1.0f; 

			return matIden;
		}

		static Matrix4 RotationX(float radian)
		{
			Matrix4 matIden = Matrix4::Identity;
			matIden._11 = 1.0f; 
			matIden._22 = cos(radian);
			matIden._23 = sin(radian);
			matIden._32 = -sin(radian);
			matIden._33 = cos(radian);

			return matIden;
		};
		static Matrix4 RotationY(float radian)
		{
			Matrix4 matIden = Matrix4::Identity;
			matIden._11 = cos(radian);
			matIden._13 = -sin(radian);
			matIden._31 = sin(radian);
			matIden._33 = cos(radian);

			return matIden;
		};
		static Matrix4 RotationZ(float radian)
		{
			Matrix4 matIden = Matrix4::Identity;
			matIden._11 = cos(radian);
			matIden._12 = sin(radian);
			matIden._21 = -sin(radian);
			matIden._22 = cos(radian); 

			return matIden;
		};

		static Matrix4 Scaling(Vector3& s)
		{
			return
			{
				s.x, 0.0f, 0.0f, 0.0f,
				0.0f, s.y, 0.0f, 0.0f,
				0.0f, 0.0f, s.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		};

		static Matrix4 RotationAxis(const Vector3& axis, float radian);
		static Matrix4 RotationQuaternion(const Quaternion& q);
		static Matrix4 Transform(const Vector3& translation, const Quaternion& rotation, const Vector3& scale);

		static Matrix4 Scaling(Vector3 scale)
		{
			Matrix4 matIden = Matrix4::Identity;
			matIden._11 *= scale.x; 
			matIden._22 *= scale.y;
			matIden._33 *= scale.z;
			matIden._44 = 1.0f; 

			return matIden; 

		};

		
	};

} //namespace MAGE::Math