#pragma once

#include<cstdio>
#include<iostream>
#include<vector>

class Kalyna
{
private:
	int Nk;
	int Nb;
	int Nr;

	std::vector<std::vector<uint64_t>> m_roundKeys;
	std::vector<uint64_t> m_key;
	int blockBytesLen;

private:
	void SubBytes(std::vector<uint64_t>& state);
	void ShiftRows(std::vector<uint64_t>& state);
	void MixColumns(std::vector<uint64_t>& state);

	void InvSubBytes(std::vector<uint64_t>& state);
	void InvShiftRows(std::vector<uint64_t>& state);
	void InvMixColumns(std::vector<uint64_t>& state);

	uint8_t MultGF(uint8_t f, uint8_t s);
	std::vector<uint64_t> MatrixMult(const std::vector<uint64_t>& first, const std::vector<uint64_t>& second);
	void AddRoundKey(std::vector<uint64_t>& state, const int round);
	void SubRoundKey(std::vector<uint64_t>& state, const int round);
	void AddRoundKeyExpand(std::vector<uint64_t>& state, const std::vector<uint64_t>& values);
	void XorRoundKey(std::vector<uint64_t>& state, const int round);
	void XorRoundKeyExpand(std::vector<uint64_t>& state, const std::vector<uint64_t>& values);
	void Rotate(std::vector<uint64_t>& state);
	void ShiftLeft(std::vector<uint64_t>& state);
	void RotateLeft(std::vector<uint64_t>& state);

	void EncipherRound(std::vector<uint64_t>& state);
	void DecipherRound(std::vector<uint64_t>& state);

	void KeyExpandKt(std::vector<uint64_t>& state, std::vector<uint64_t>& kt);
	void KeyExpandEven(std::vector<uint64_t>& state, std::vector<uint64_t>& kt);
	void KeyExpandOdd();
	void KalynaKeyExpand(std::vector<uint64_t>& state);

	std::vector<uint64_t> KalynaEncipher(std::vector<uint64_t>& input);
	std::vector<uint64_t> KalynaDecipher(std::vector<uint64_t>& input);

public:
	Kalyna(const int blockSize, const int keySize, const std::vector<uint64_t>& key);
	void Encrypt(std::string inputFilePath, std::string outputFilePath);
	void Decrypt(std::string inputFilePath, std::string outputFilePath);
};