#pragma once

#include "tables.h"
#include <cstring>
#include <iostream>
#include <stdio.h>

class AES
{
private:
	int Nb;
	int Nk;
	int Nr;

	unsigned int blockBytesLen;

	uint8_t xTime(const uint8_t& value);
	uint8_t MultBy(const uint8_t& input, const uint8_t& value);
	void LeftShiftColumn(std::vector<uint8_t>& column);
	void SubColumn(std::vector<uint8_t>& column);
	std::vector<uint8_t> GetRConColumn(const int& index);
	std::vector<uint8_t> XorColumns(const std::vector<uint8_t>& first,
		const std::vector<uint8_t>& second, const std::vector<uint8_t>& third);
	void PushPaddingZeros(std::vector<uint8_t>& msg);

	void SubBytes(std::vector<std::vector<uint8_t>>& state);
	void ShiftRows(std::vector<std::vector<uint8_t>>& state);
	void ShiftRow(std::vector<uint8_t>& row, const int& step);
	void MixColumns(std::vector<std::vector<uint8_t>>& state);
	void MixColumn(std::vector<uint8_t>& column);
	void KeyExpansion(const std::vector<uint8_t>& key, std::vector<std::vector<uint8_t>>& res);
	void AddRoundKey(std::vector<std::vector<uint8_t>>& state, const std::vector<uint8_t>& key);

	std::vector<uint8_t> EncryptBlock(const std::vector<uint8_t>& input, 
		const std::vector<std::vector<uint8_t>>& roundKeys);
public:
	std::vector<uint8_t> Encrypt(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key);

	AES();
};
