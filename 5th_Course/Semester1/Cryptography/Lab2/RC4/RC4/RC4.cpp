#include "RC4.h"
#include <fstream>
#include <chrono>
#include <iostream>

RC4::RC4(const std::string key)
{
	ksa(key);
}

void RC4::ksa(const std::string key)
{
	S = std::vector<unsigned char>(256);
	for (int i = 0; i < 256; ++i)
		S[i] = i;

	int keyLength = key.size();

	int j = 0;
	for (int i = 0; i < 256; ++i)
	{
		j = (j + S[i] + key[i % keyLength]) % 256;
		std::swap(S[i], S[j]);
	}
}

std::string RC4::prga(const std::string input)
{
	auto tempS = S;

	std::string res(input.size(), '\0');
	int i = 0;
	int j = 0;

	for (size_t r = 0; r < input.size(); ++r)
	{
		i = (i + 1) % 256;
		j = (j + tempS[i]) % 256;

		std::swap(tempS[i], tempS[j]);
		
		res[r] = tempS[(tempS[i] + tempS[j]) % 256] ^ input[r];
	}

	return res;
}

std::string RC4::EncryptMessage(const std::string input)
{
	return prga(input);
}

std::string RC4::DecryptMessage(const std::string input)
{
	return prga(input);
}

void RC4::Encrypt(std::string inputFilePath, std::string outputFilePath)
{
	std::ifstream inputFile(inputFilePath, std::ios::binary);
	if (!inputFile.is_open())
		throw "Something went wrong while opening input file.";

	std::ofstream outputFile(outputFilePath, std::ios::binary);
	if (!outputFile.is_open())
		throw "Something went wrong while opening output file.";

	inputFile.seekg(0, inputFile.end);
	long long fileLength = inputFile.tellg();
	inputFile.seekg(0, inputFile.beg);

	auto startTime = std::chrono::system_clock::now();

	int maxSize = 256;
	std::string message;
	message.resize(maxSize);

	for (int i = 0; i < fileLength; i += maxSize)
	{
		inputFile.read(message.data(), maxSize);
		auto encrypted = EncryptMessage(message);
		outputFile.write(encrypted.data(), maxSize);
	}

	std::cout << "Encrypted at " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count()
		<< " milliseconds" << std::endl;

	inputFile.close();
	outputFile.close();
}

void RC4::Decrypt(std::string inputFilePath, std::string outputFilePath)
{
	std::ifstream inputFile(inputFilePath, std::ios::binary);
	if (!inputFile.is_open())
		throw "Something went wrong while opening input file.";

	std::ofstream outputFile(outputFilePath, std::ios::binary);
	if (!outputFile.is_open())
		throw "Something went wrong while opening output file.";

	inputFile.seekg(0, inputFile.end);
	long long fileLength = inputFile.tellg();
	inputFile.seekg(0, inputFile.beg);

	auto startTime = std::chrono::system_clock::now();

	int maxSize = 256;
	std::string message;
	message.resize(maxSize);

	for (int i = 0; i < fileLength; i += maxSize)
	{
		inputFile.read(message.data(), maxSize);
		auto decrypted = DecryptMessage(message);
		outputFile.write(decrypted.data(), maxSize);
	}

	std::cout << "Encrypted at " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count()
		<< " milliseconds" << std::endl;

	inputFile.close();
	outputFile.close();
}