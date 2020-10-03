#include <stdio.h>
#include "AES.h"
#include <iostream>
#include <fstream>
#include <chrono>

void generate()
{
	std::ofstream file("../../TestFiles/testFile1Gb.txt");
	for (int i = 0; i < 1000; ++i)
	{
		std::string s;
		for (int j = 0; j < 1024 * 1024; ++j)
		{
			s += 0x30 + (uint8_t)(rand() % 9);
		}
		file.write(s.c_str(), s.size());
	}
}

int main()
{
	std::vector<uint8_t> key =
	{
		0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
		0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
	};

	AES aes(128, key);
	auto start = std::chrono::system_clock::now();
	aes.Encrypt("../../TestFiles/testFile1Mb.txt", "../../Results/encrypted1Mb.txt");
	std::cout << "Encrypted at: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() << std::endl;
	
	start = std::chrono::system_clock::now();
	aes.Decrypt("../../Results/encrypted1Mb.txt", "../../Results/decrypted1Mb.txt");
	std::cout << "Decrypted at: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() << std::endl;

	return 0;
}