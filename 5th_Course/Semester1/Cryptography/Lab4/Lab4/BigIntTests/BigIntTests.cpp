#include "pch.h"
#include "CppUnitTest.h"
#include "bigint.h"
#include "additional.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BigIntTests
{
	TEST_CLASS(BigIntTests)
	{
	public:
		
		TEST_METHOD(BigIntDivideTest1)
		{
			a = 99;
			b = 2;
			exp = 49;
			Assert::IsTrue(a / b == exp);
		}

		TEST_METHOD(BigIntDivideTest2)
		{
			a = 100;
			b = 100;
			exp = 1;
			Assert::IsTrue(a / b == exp);
		}

		TEST_METHOD(BigIntDivideTest3)
		{
			a = 100;
			b = 1;
			exp = 100;
			Assert::IsTrue(a / b == exp);
		}

		TEST_METHOD(BigIntDivideTest4)
		{
			a = BigInt("12312354647673453227");
			b = BigInt("9385728394729");
			exp = BigInt("1311816");
			BigInt res = a / b;
			Assert::IsTrue(res == exp);
		}

		TEST_METHOD(BigIntDivideTest5)
		{
			a = BigInt("1231235464767345322732642376821746");
			b = BigInt("222");
			exp = BigInt("5546105697150204156453344039737");
			BigInt res = a / b;
			Assert::IsTrue(res == exp);
		}

		TEST_METHOD(BigIntDivideTest6)
		{
			a = BigInt("1231235464767345322732642376821746");
			b = BigInt("222");
			exp = BigInt("0");
			BigInt res = b / a;
			Assert::IsTrue(res == exp);
		}

		TEST_METHOD(EEATest1)
		{
			a = 2;
			b = 3;
			exp = 2;
			BigInt res = AdditionalFuncs::ExtendedEuclidianAlgo(2, 3);

			Assert::IsTrue(res == exp);
		}

		TEST_METHOD(PowerModTest1)
		{
			BigInt base = 3;
			BigInt power = 5;
			BigInt mod = 2;
			BigInt expected = 1;
			BigInt actual = base.powerMod(power, mod);
			Assert::IsTrue(expected == actual);
		}

		TEST_METHOD(PowerModTest2)
		{
			BigInt base = 3;
			BigInt power = 5;
			BigInt mod = 3;
			BigInt expected = 0;
			BigInt actual = base.powerMod(power, mod);
			Assert::IsTrue(expected == actual);
		}

		TEST_METHOD(PowerModTest3)
		{
			BigInt base = 3;
			BigInt power = 5;
			BigInt mod = 5;
			BigInt expected = 3;
			BigInt actual = base.powerMod(power, mod);
			Assert::IsTrue(expected == actual);
		}

		TEST_METHOD(PowerModTest4)
		{
			BigInt base = 3;
			BigInt power = 5;
			BigInt mod = 30;
			BigInt expected = 3;
			BigInt actual = base.powerMod(power, mod);
			Assert::IsTrue(expected == actual);
		}

		TEST_METHOD(ModTest1)
		{
			BigInt base = 15;
			BigInt mod = 3;
			BigInt expected = 0;
			BigInt actual = base % mod;
			Assert::IsTrue(expected == actual);
		}

		TEST_METHOD(ModTest2)
		{
			BigInt base = 15;
			BigInt mod = 4;
			BigInt expected = 3;
			BigInt actual = base % mod;
			Assert::IsTrue(expected == actual);
		}

		TEST_METHOD(ModTest3)
		{
			BigInt base = 15;
			BigInt mod = 5;
			BigInt expected = 0;
			BigInt actual = base % mod;
			Assert::IsTrue(expected == actual);
		}

		TEST_METHOD(ModTest4)
		{
			BigInt base = 15;
			BigInt mod = 1;
			BigInt expected = 0;
			BigInt actual = base % mod;
			Assert::IsTrue(expected == actual);
		}

		TEST_METHOD(ModTest5)
		{
			BigInt base = 15;
			BigInt mod = 2;
			BigInt expected = 1;
			BigInt actual = base % mod;
			Assert::IsTrue(expected == actual);
		}

		TEST_METHOD(ModTest6)
		{
			BigInt base = 15;
			BigInt mod = 15;
			BigInt expected = 0;
			BigInt actual = base % mod;
			Assert::IsTrue(expected == actual);
		}

		TEST_METHOD(ModTest7)
		{
			BigInt base = -15;
			BigInt mod = 4;
			BigInt expected = 1;
			BigInt actual = base % mod;
			Assert::IsTrue(expected == actual);
		}

		TEST_METHOD(ToBinaryTest1)
		{
			a = 13;
			auto binary = a.toBinary();
			std::vector<BigInt> expected = { 1, 1, 0, 1 };

			for (int i = 0; i < expected.size(); ++i)
			{
				Assert::IsTrue(binary[i] == expected[i]);
			}
		}

		TEST_METHOD(ToBinaryTest2)
		{
			a = 435761;
			auto binary = a.toBinary();
			std::vector<BigInt> expected = { 1,1,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,1 };

			for (int i = 0; i < expected.size(); ++i)
			{
				Assert::IsTrue(binary[i] == expected[i]);
			}
		}
	private:
		BigInt a, b, exp;
	};
}
