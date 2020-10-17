#pragma once

#include <vector>
#include <string>

class RC4
{
private:
	std::vector<unsigned char> S;
	void ksa(const std::string key);
	std::string prga(const std::string input);
	std::string EncryptMessage(const std::string input);
	std::string DecryptMessage(const std::string input);

public:
	RC4(const std::string key);
	void Encrypt(std::string inputFilePath, std::string outputFilePath);
	void Decrypt(std::string inputFilePath, std::string outputFilePath);
};

