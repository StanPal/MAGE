#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace MAGE::Math;

namespace MathTest
{		
	TEST_CLASS(Vector3Test)
	{
	public:
		
		TEST_METHOD(TestStatics)
		{
			Vector3 zero = Vector3::Zero;
			Assert::AreEqual(zero.x, 0.0f);
			Assert::AreEqual(zero.y, 0.0f);
			Assert::AreEqual(zero.z, 0.0f);
			
			Vector3 one = Vector3::One;
			Assert::AreEqual(one.x, 1.0f);
			Assert::AreEqual(one.y, 1.0f);
			Assert::AreEqual(one.z, 1.0f);


			// TODO: Your test code here
		}

		TEST_METHOD(TestAdditions)
		{
			Vector3 a{ 1.0f,2.0f,3.0f };
			Vector3 b{ 4.0f,5.0f,6.0f };
			Vector3 c = a + b;
			Assert::AreEqual(c.x, 5.0f);
			Assert::AreEqual(c.y, 7.0f);
			Assert::AreEqual(c.z, 9.0f);

			a += b; 
			Assert::AreEqual(a.x, 5.0f);
			Assert::AreEqual(a.y, 7.0f);
			Assert::AreEqual(a.z, 9.0f);
	
		}

		TEST_METHOD(TestSubtractions)
		{
			Vector3 a{ 1.0f,2.0f,3.0f };
			Vector3 b{ 4.0f,5.0f,6.0f };
			Vector3 c = b - a;
			Assert::AreEqual(c.x, 3.0f);
			Assert::AreEqual(c.y, 3.0f);
			Assert::AreEqual(c.z, 3.0f);

			b -= a;
			Assert::AreEqual(b.x, 3.0f);
			Assert::AreEqual(b.y, 3.0f);
			Assert::AreEqual(b.z, 3.0f);
		}

		TEST_METHOD(TestMultiplication)
		{
			Vector3 a{ 1.0f,2.0f,3.0f };
			Vector3 b{ 4.0f,5.0f,6.0f };
			Vector3 c = b * a;

			Assert::AreEqual(c.x, 4.0f);
			Assert::AreEqual(c.y, 10.0f);
			Assert::AreEqual(c.z, 18.0f);

			a *= b;
			Assert::AreEqual(a.x, 4.0f);
			Assert::AreEqual(a.y, 10.0f);
			Assert::AreEqual(a.z, 18.0f);
		}

		TEST_METHOD(TestDivision)
		{
			Vector3 a{ 1.0f,2.0f,3.0f };
			Vector3 b{ 4.0f,8.0f,6.0f };
			Vector3 c = b / a;
			Assert::AreEqual(c.x, 4.0f);
			Assert::AreEqual(c.y, 4.0f);
			Assert::AreEqual(c.z, 2.0f);

			b /= a;
			Assert::AreEqual(b.x, 4.0f);
			Assert::AreEqual(b.y, 4.0f);
			Assert::AreEqual(b.z, 2.0f);
		}

		TEST_METHOD(TestDotProduct)
		{
			Vector3 a{ 1.0f,2.0f,3.0f };
			Vector3 b{ 3.0f,4.0f,5.0f };
			float result = MAGE::Math::Dot(a,b);
			Assert::AreEqual(result, 26.0f);
		}

		TEST_METHOD(TestCrossProduct)
		{
			Vector3 a{ 1.0f,2.0f,3.0f };
			Vector3 b{ 3.0f,4.0f,5.0f };
			Vector3 c = MAGE::Math::Cross(a, b);

			Assert::AreEqual(c.x, -2.0f);
			Assert::AreEqual(c.y, 4.0f);
			Assert::AreEqual(c.z, -2.0f);
		}

		TEST_METHOD(TestMagnitudesqrd)
		{
			Vector3 a{ 3.0f,4.0f,5.0f };
			float magnitudesqrd = 0.0f; 
			magnitudesqrd = MAGE::Math::MagnitudeSqrd(a);
			Assert::AreEqual(magnitudesqrd, 50.0f);
		}

		TEST_METHOD(TestMagnitude)
		{
			Vector3 a{ 3.0f,4.0f,5.0f };
			float magnitude = 0.0f;
			magnitude = MAGE::Math::Magnitude(a);

			Assert::AreEqual(magnitude, sqrt(50.0f));
		}

		TEST_METHOD(TestNormalize)
		{
			Vector3 a{ 3.0f,4.0f,5.0f };
			float magnitude = 0.0f;
			magnitude = MAGE::Math::Magnitude(a);
			Vector3 NormVect = MAGE::Math::Normalize(a);

			Assert::AreEqual(NormVect.x, a.x / magnitude);
			Assert::AreEqual(NormVect.y, a.y / magnitude);
			Assert::AreEqual(NormVect.z, a.z / magnitude);
		}
	};
}