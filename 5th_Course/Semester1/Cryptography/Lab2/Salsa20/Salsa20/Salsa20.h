#pragma once
#include <vector>
#include <fstream>
#include <string>

class Salsa20
{
private:
	uint32_t Rotate(uint32_t value, uint8_t shift);
	void QR(uint32_t& y0, uint32_t& y1, uint32_t& y2, uint32_t& y3);
	void SalsaBlock(uint32_t* state, size_t state_size, size_t stream_pos);
	std::string CryptString(const std::string& input);
	void ExpandKey();

	const uint8_t m_key32[16] = { 'e','x','p','a','n','d',' ','3','2','-','b','y','t','e',' ','k' };

	std::string m_key;
	std::vector<uint32_t> m_init_key;

public:

	Salsa20(std::string key);
	void Encrypt(const std::string inputFilePath, const std::string outputFilePath);
	void Decrypt(const std::string inputFilePath, const std::string outputFilePath);
};

