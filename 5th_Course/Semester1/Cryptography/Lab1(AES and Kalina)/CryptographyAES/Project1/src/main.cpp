#include <stdio.h>
#include "AES.h"
#include <iostream>
#include <fstream>
#include <chrono>

void generate()
{
	std::ofstream file("../../TestFiles/testFileSmall.txt");
	for (int i = 0; i < 1; ++i)
	{
		std::string s;
		for (int j = 0; j < 1024; ++j)
		{
			s += 0x30 + rand()%9;
		}
		file.write(s.c_str(), s.size());
	}
}

int main()
{
	std::vector<uint8_t> key =
	{
		0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
		0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35
	};

	AES aes(128, key);
	aes.Encrypt("../../TestFiles/testFile100Mb.txt", "../../Results/encrypted100Mb.txt");
	aes.Decrypt("../../Results/encrypted100Mb.txt", "../../Results/decrypted100Mb.txt");

	return 0;
}