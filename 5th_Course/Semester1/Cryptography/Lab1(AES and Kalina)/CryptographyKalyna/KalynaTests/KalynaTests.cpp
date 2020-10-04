#include "pch.h"
#include "CppUnitTest.h"
#include <fstream>
#include "../CryptographyKalyna/src/kalyna.h"
#include "../CryptographyKalyna/src/kalyna.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace KalynaTests
{
	TEST_CLASS(KalynaTests)
	{
	public:
		
		bool IsFilesEqual(std::string fPath, std::string sPath)
		{
			std::ifstream fFile(fPath, std::ios::binary);
			if (!fFile.is_open())
				throw "Can't open first file";

			std::ifstream sFile(sPath, std::ios::binary);
			if (!sFile.is_open())
				throw "Can't open second file";

			long long fLength, sLength;

			fFile.seekg(0, fFile.end);
			fLength = fFile.tellg();
			fFile.seekg(0, fFile.beg);

			sFile.seekg(0, sFile.end);
			sLength = sFile.tellg();
			sFile.seekg(0, sFile.beg);

			if (fLength != sLength)
				return false;

			for (long long i = 0; i < fLength; ++i)
				if (fFile.get() != sFile.get())
					return false;
			return true;
		}

		std::vector<uint64_t> key128 =
		{
			0x0001020304050607, 0x08090A0B0C0D0E0F
		};

		std::vector<uint64_t> key256 =
		{
			0x0001020304050607, 0x08090A0B0C0D0E0F,
			0x0001020304050607, 0x08090A0B0C0D0E0F
		};

		std::vector<uint64_t> key512 =
		{
			0x0001020304050607, 0x08090A0B0C0D0E0F,
			0x0001020304050607, 0x08090A0B0C0D0E0F,
			0x0001020304050607, 0x08090A0B0C0D0E0F,
			0x0001020304050607, 0x08090A0B0C0D0E0F
		};

		TEST_METHOD(KalynaTest1Mb_128_128)
		{
			Kalyna kalyna(128, 128, key128);
			kalyna.Encrypt("../../../TestFiles/testFile1Mb.txt", "../../../Results/encrypted1Mb.txt");
			kalyna.Decrypt("../../../Results/encrypted1Mb.txt", "../../../Results/decrypted1Mb.txt");

			Assert::IsTrue(IsFilesEqual("../../../TestFiles/testFile1Mb.txt", "../../../Results/decrypted1Mb.txt"));
		}

		TEST_METHOD(KalynaTest1Mb_128_256)
		{
			Kalyna kalyna(128, 256, key256);
			kalyna.Encrypt("../../../TestFiles/testFile1Mb.txt", "../../../Results/encrypted1Mb.txt");
			kalyna.Decrypt("../../../Results/encrypted1Mb.txt", "../../../Results/decrypted1Mb.txt");

			Assert::IsTrue(IsFilesEqual("../../../TestFiles/testFile1Mb.txt", "../../../Results/decrypted1Mb.txt"));
		}

		TEST_METHOD(KalynaTest1Mb_256_256)
		{
			Kalyna kalyna(256, 256, key256);
			kalyna.Encrypt("../../../TestFiles/testFile1Mb.txt", "../../../Results/encrypted1Mb.txt");
			kalyna.Decrypt("../../../Results/encrypted1Mb.txt", "../../../Results/decrypted1Mb.txt");

			Assert::IsTrue(IsFilesEqual("../../../TestFiles/testFile1Mb.txt", "../../../Results/decrypted1Mb.txt"));
		}

		TEST_METHOD(KalynaTest1Mb_256_512)
		{
			Kalyna kalyna(256, 512, key512);
			kalyna.Encrypt("../../../TestFiles/testFile1Mb.txt", "../../../Results/encrypted1Mb.txt");
			kalyna.Decrypt("../../../Results/encrypted1Mb.txt", "../../../Results/decrypted1Mb.txt");

			Assert::IsTrue(IsFilesEqual("../../../TestFiles/testFile1Mb.txt", "../../../Results/decrypted1Mb.txt"));
		}

		TEST_METHOD(KalynaTest1Mb_512_512)
		{
			Kalyna kalyna(512, 512, key512);
			kalyna.Encrypt("../../../TestFiles/testFile1Mb.txt", "../../../Results/encrypted1Mb.txt");
			kalyna.Decrypt("../../../Results/encrypted1Mb.txt", "../../../Results/decrypted1Mb.txt");

			Assert::IsTrue(IsFilesEqual("../../../TestFiles/testFile1Mb.txt", "../../../Results/decrypted1Mb.txt"));
		}

	};
}
