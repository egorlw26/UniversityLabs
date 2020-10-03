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

private:
	void SubBytes(std::vector<uint64_t>& state);
	void ShiftRows(std::vector<uint64_t>& state);
	void MixColumns(std::vector<uint64_t>& state);

	void InvSubBytes(std::vector<uint64_t>& state);
	void InvShiftRows(std::vector<uint64_t>& state);
	void InvMixColumns(std::vector<uint64_t>& state);

	uint8_t MultGF(uint8_t f, uint8_t s);
	std::vector<uint64_t> MatrixMult(const std::vector<uint64_t>& first, const std::vector<uint64_t>& second);
	void AddRoundKey(std::vector<uint64_t>& state, const std::vector<std::vector<uint64_t>>& roundKeys, const int round);
	void SubRoundKey(std::vector<uint64_t>& state, const std::vector<std::vector<uint64_t>>& roundKeys, const int round);
	void AddRoundKeyExpand(std::vector<uint64_t>& state, const std::vector<uint64_t>& values);
	void XorRoundKey(std::vector<uint64_t>& state, const std::vector<std::vector<uint64_t>>& roundKeys, const int round);
	void XorRoundKeyExpand(std::vector<uint64_t>& state, const std::vector<uint64_t>& values);
	void Rotate(std::vector<uint64_t>& state);
	void ShiftLeft(std::vector<uint64_t>& state);
	void RotateLeft(std::vector<uint64_t>& state);

	void EncipherRound(std::vector<uint64_t>& state);
	void DecipherRound(std::vector<uint64_t>& state);

	void KeyExpandKt(std::vector<uint64_t>& state, std::vector<uint64_t>& key, std::vector<uint64_t>& kt);
	void KeyExpandEven(std::vector<uint64_t>& state, std::vector<uint64_t>& key, std::vector<std::vector<uint64_t>>& round_keys, std::vector<uint64_t>& kt);
	void KeyExpandOdd(std::vector<std::vector<uint64_t>>& round_keys);

	void KalynaEncipher(std::vector<uint64_t>& state, std::vector<uint64_t>& msg, std::vector<std::vector<uint64_t>>& roundKeys, std::vector<uint64_t>& cipher);
	void KalynaDecipher(std::vector<uint64_t>& state, std::vector<uint64_t>& msg, std::vector<std::vector<uint64_t>>& roundKeys, std::vector<uint64_t>& cipher);

public:
	Kalyna(const int blockSize, const int keySize);

};