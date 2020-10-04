#include "pch.h"
#include "CppUnitTest.h"
#include "../Project1/src/AES.h"
#include "../Project1/src/AES.cpp"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AESTests
{
	TEST_CLASS(AESTests)
	{
	public:
		std::vector<uint8_t> key128 =
		{
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
			0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35
		};

		std::vector<uint8_t> key192 =
		{
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
			0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
		};

		std::vector<uint8_t> key256 =
		{
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
			0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
			0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
			0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
		};

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

		TEST_METHOD(Test1MbEncryptDecrypt128)
		{
			AES aes(128, key128);
			aes.Encrypt("../../../TestFiles/testFile1Mb.txt", "../../../Results/encrypted1Mb.txt");
			aes.Decrypt("../../../Results/encrypted1Mb.txt", "../../../Results/decrypted1Mb.txt");

			Assert::IsTrue(IsFilesEqual("../../../TestFiles/testFile1Mb.txt", "../../../Results/decrypted1Mb.txt"));
		}

		TEST_METHOD(Test1MbEncryptDecrypt256)
		{
			AES aes(256, key256);
			aes.Encrypt("../../../TestFiles/testFile1Mb.txt", "../../../Results/encrypted1Mb.txt");
			aes.Decrypt("../../../Results/encrypted1Mb.txt", "../../../Results/decrypted1Mb.txt");

			Assert::IsTrue(IsFilesEqual("../../../TestFiles/testFile1Mb.txt", "../../../Results/decrypted1Mb.txt"));
		}

		TEST_METHOD(Test10MbEncryptDecrypt192)
		{
			AES aes(192, key192);
			aes.Encrypt("../../../TestFiles/testFile10Mb.txt", "../../../Results/encrypted10Mb.txt");
			aes.Decrypt("../../../Results/encrypted10Mb.txt", "../../../Results/decrypted10Mb.txt");

			Assert::IsTrue(IsFilesEqual("../../../TestFiles/testFile10Mb.txt", "../../../Results/decrypted10Mb.txt"));
		}
	};
}
