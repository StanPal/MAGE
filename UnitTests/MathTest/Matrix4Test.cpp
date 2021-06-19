#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace MAGE::Math;

namespace MathTest
{
	TEST_CLASS(Matrix4Test)
	{
	public:
		TEST_METHOD(TestStatics)
		{
			Matrix4 identityMat = Matrix4::Identity;
			//Row 1 
			Assert::AreEqual(identityMat._11, 1.0f);
			Assert::AreEqual(identityMat._12, 0.0f);
			Assert::AreEqual(identityMat._13, 0.0f);
			Assert::AreEqual(identityMat._14, 0.0f);
			//Row 2
			Assert::AreEqual(identityMat._21, 0.0f);
			Assert::AreEqual(identityMat._22, 1.0f);
			Assert::AreEqual(identityMat._23, 0.0f);
			Assert::AreEqual(identityMat._24, 0.0f);
			//Row 3
			Assert::AreEqual(identityMat._31, 0.0f);
			Assert::AreEqual(identityMat._32, 0.0f);
			Assert::AreEqual(identityMat._33, 1.0f);
			Assert::AreEqual(identityMat._34, 0.0f);
			//Row 4
			Assert::AreEqual(identityMat._41, 0.0f);
			Assert::AreEqual(identityMat._42, 0.0f);
			Assert::AreEqual(identityMat._43, 0.0f);
			Assert::AreEqual(identityMat._44, 1.0f);

		}
		TEST_METHOD(TestTranspose)
		{
			Matrix4 mat{ 2.0f,3.0f,4.0f,0.0f,
						5.0f,6.0f,7.0f,0.0f,
						8.0f,9.0f,10.0f,0.0f,
						0.0f,0.0f,0.0f,1.0f
			};
			Matrix4 transposeMat = MAGE::Math::Transpose(mat);

			Assert::AreEqual(transposeMat._11, 2.0f);
			Assert::AreEqual(transposeMat._12, 5.0f);
			Assert::AreEqual(transposeMat._13, 8.0f);
			Assert::AreEqual(transposeMat._14, 0.0f);
			//Row 2			 
			Assert::AreEqual(transposeMat._21, 3.0f);
			Assert::AreEqual(transposeMat._22, 6.0f);
			Assert::AreEqual(transposeMat._23, 9.0f);
			Assert::AreEqual(transposeMat._24, 0.0f);
			//Row 3			 
			Assert::AreEqual(transposeMat._31, 4.0f);
			Assert::AreEqual(transposeMat._32, 7.0f);
			Assert::AreEqual(transposeMat._33, 10.0f);
			Assert::AreEqual(transposeMat._34, 0.0f);
			//Row 4			 
			Assert::AreEqual(transposeMat._41, 0.0f);
			Assert::AreEqual(transposeMat._42, 0.0f);
			Assert::AreEqual(transposeMat._43, 0.0f);
			Assert::AreEqual(transposeMat._44, 1.0f);
		}

		TEST_METHOD(TestRotateX)
		{
			Matrix4 rotateX = Matrix4::RotationX(90.0f);

			Assert::AreEqual(rotateX._11, 1.0f);
			Assert::AreEqual(rotateX._22, cos(90.0f));
			Assert::AreEqual(rotateX._23, -sin(90.0f));
			Assert::AreEqual(rotateX._32, sin(90.0f));
			Assert::AreEqual(rotateX._33, cos(90.0f));

		}

		TEST_METHOD(TestRotateY)
		{
			Matrix4 rotateY = Matrix4::RotationY(90.0f);
			Assert::AreEqual(rotateY._11, cos(90.0f));
			Assert::AreEqual(rotateY._13, sin(90.0f));
			Assert::AreEqual(rotateY._22, 1.0f);
			Assert::AreEqual(rotateY._31, -sin(90.0f));
			Assert::AreEqual(rotateY._33, cos(90.0f));
		}

		TEST_METHOD(TestRotateZ)
		{
			Matrix4 rotateZ = Matrix4::RotationZ(90.0f);
			Assert::AreEqual(rotateZ._11, cos(90.0f));
			Assert::AreEqual(rotateZ._12, -sin(90.0f));
			Assert::AreEqual(rotateZ._21, sin(90.0f));
			Assert::AreEqual(rotateZ._22, cos(90.0f));
			Assert::AreEqual(rotateZ._33, 1.0f);
		}

		TEST_METHOD(TestRotationAxis)
		{

		}
		TEST_METHOD(TestTranslation)
		{
			Vector3 translateVect{ 1.0f,2.0f,3.0f };
			Matrix4 matTrans = Matrix4::Translation(translateVect);
			Assert::AreEqual(matTrans._11, 1.0f);
			Assert::AreEqual(matTrans._22, 2.0f);
			Assert::AreEqual(matTrans._33, 3.0f);
			Assert::AreEqual(matTrans._44, 1.0f);

		}

		TEST_METHOD(TestDeterminant)
		{
			Matrix4 Mat4{ 2.0f,2.0f,3.0f,0.0f,
						4.0f,5.0f,6.0f,0.0f,
						7.0f,8.0f,9.0f,0.0f,
						0.0f,0.0f,0.0f,1.0f };
			float determinant = MAGE::Math::Determinant(Mat4);

			Assert::AreEqual(determinant, -3.0f);

		}

		TEST_METHOD(TestInverse)
		{

		}

		

	};
}
