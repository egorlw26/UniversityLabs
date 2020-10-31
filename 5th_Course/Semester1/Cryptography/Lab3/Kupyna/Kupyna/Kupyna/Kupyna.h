#pragma once
#include <vector>
#include <string>

class Kupyna
{
private:
	std::vector<std::vector<uint8_t>> m_state;
	size_t m_nbytes;
	size_t m_hash_bits;
	int m_columns;
	int m_rounds;

	std::string m_result;

	void SubBytes(std::vector<std::vector<uint8_t>>& state);
	void ShiftBytes(std::vector<std::vector<uint8_t>>& state);
	void MixColumns(std::vector<std::vector<uint8_t>>& state);
	void AddRoundConstantP(std::vector<std::vector<uint8_t>>& state, const int round);
	void AddRoundConstantQ(std::vector<std::vector<uint8_t>>& state, const int round);
	void P(std::vector<std::vector<uint8_t>>& state);
	void Q(std::vector<std::vector<uint8_t>>& state);
	void Pad(std::string& msg);
	void Digest(const std::string& msg);
	void OutputTransform();
	int KupynaInit(const size_t hash_nbits);
	void KupynaHash(const size_t hash_bits, const std::string& input);
	void Trunc();

public:
	void showResult();
	std::string getResultString();
	Kupyna(const size_t hash_bits, const std::string& input);
};

