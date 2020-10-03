#include "AES.h"
#include <fstream>

AES::AES(int keyLen, const std::vector<uint8_t>& key)
{
	Nb = 4;
	switch (keyLen)
	{
	case 128:
		Nk = 4;
		Nr = 10;
		break;
	case 192:
		Nk = 6;
		Nr = 12;
		break;
	case 256:
		Nk = 8;
		Nr = 14;
		break;
	default:
		throw "Incorrect length of key";
		break;
	}

	blockBytesLen = 4 * this->Nb * sizeof(unsigned char);

	std::vector<std::vector<uint8_t>> roundKeys(4, std::vector<uint8_t>(Nb * (Nr + 1)));
	KeyExpansion(key, roundKeys);
	m_roundKeys = roundKeys;
};

void AES::SubBytes(std::vector<std::vector<uint8_t>>& state)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < Nb; ++j)
		{
			auto t = state[i][j];
			state[i][j] = sBox[t];
		}
}

void AES::ShiftRows(std::vector<std::vector<uint8_t>>& state)
{
	ShiftRow(state[1], 1);
	ShiftRow(state[2], 2);
	ShiftRow(state[3], 3);
}

void AES::ShiftRow(std::vector<uint8_t>& row, const int& step)
{
	auto row_copy = row;
	for (int i = 0; i < Nb; ++i)
		row[i] = row_copy[(i + step) % Nb];
}

void AES::MixColumns(std::vector<std::vector<uint8_t>>& state)
{
	std::vector<uint8_t> temp_column(4);
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
			temp_column[j] = state[j][i];
		MixColumn(temp_column);
		for (int j = 0; j < 4; ++j)
			state[j][i] = temp_column[j];
	}
}

void AES::MixColumn(std::vector<uint8_t>& column)
{
	std::vector<uint8_t> a = column;
	column[0] = xTime(a[0]) ^ MultBy(a[1], 0x03) ^ a[2] ^ a[3];
	column[1] = a[0] ^ xTime(a[1]) ^ MultBy(a[2], 0x03) ^ a[3];
	column[2] = a[0] ^ a[1] ^ xTime(a[2]) ^ MultBy(a[3], 0x03);
	column[3] = MultBy(a[0], 0x03) ^ a[1] ^ a[2] ^ xTime(a[3]);
}

void AES::InvSubBytes(std::vector<std::vector<uint8_t>>& state)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < Nb; ++j)
		{
			auto t = state[i][j];
			state[i][j] = invSBox[t];
		}
}

void AES::InvShiftRows(std::vector<std::vector<uint8_t>>& state)
{
	InvShiftRow(state[1], 1);
	InvShiftRow(state[2], 2);
	InvShiftRow(state[3], 3);
}

void AES::InvShiftRow(std::vector<uint8_t>& row, const int& step)
{
	auto row_copy = row;
	for (int i = 0; i < 4; ++i)
	{
		row[i] = row_copy[((i - step) % Nb + Nb) % Nb];
	}
}

void AES::InvMixColumns(std::vector<std::vector<uint8_t>>& state)
{
	std::vector<uint8_t> temp_column(4);
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
			temp_column[j] = state[j][i];
		InvMixColumn(temp_column);
		for (int j = 0; j < 4; ++j)
			state[j][i] = temp_column[j];
	}
}

void AES::InvMixColumn(std::vector<uint8_t>& column)
{
	auto a = column;
	column[0] = MultBy(a[0], 0x0e) ^ MultBy(a[1], 0x0b) ^
		MultBy(a[2], 0x0d) ^ MultBy(a[3], 0x09);
	column[1] = MultBy(a[0], 0x09) ^ MultBy(a[1], 0x0e) ^
		MultBy(a[2], 0x0b) ^ MultBy(a[3], 0x0d);
	column[2] = MultBy(a[0], 0x0d) ^ MultBy(a[1], 0x09) ^
		MultBy(a[2], 0x0e) ^ MultBy(a[3], 0x0b);
	column[3] = MultBy(a[0], 0x0b) ^ MultBy(a[1], 0x0d) ^
		MultBy(a[2], 0x09) ^ MultBy(a[3], 0x0e);
}

void AES::KeyExpansion(const std::vector<uint8_t>& key, std::vector<std::vector<uint8_t>>& w)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < Nk; ++j)
			w[i][j] = key[i + 4 * j];

	for (int i = Nk; i < Nb * (Nr + 1); ++i)
	{
		std::vector<uint8_t> nColumn(4); // Wi_m_1
		nColumn[0] = w[0][i - 1];
		nColumn[1] = w[1][i - 1];
		nColumn[2] = w[2][i - 1];
		nColumn[3] = w[3][i - 1];

		std::vector<uint8_t> Wi_m_Nk(4);
		Wi_m_Nk[0] = w[0][i - Nk];
		Wi_m_Nk[1] = w[1][i - Nk];
		Wi_m_Nk[2] = w[2][i - Nk];
		Wi_m_Nk[3] = w[3][i - Nk];

		if (i % Nk == 0)
		{
			LeftShiftColumn(nColumn);
			SubColumn(nColumn);
			nColumn = XorColumns(Wi_m_Nk, nColumn, GetRConColumn(i / Nk - 1));
		}
		else
		{
			nColumn = XorColumns(nColumn, Wi_m_Nk, { 0x00, 0x00, 0x00, 0x00 });
		}

		w[0][i] = nColumn[0];
		w[1][i] = nColumn[1];
		w[2][i] = nColumn[2];
		w[3][i] = nColumn[3];
	}
}

void AES::AddRoundKey(std::vector<std::vector<uint8_t>>& state, const std::vector<uint8_t>& key)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < Nb; ++j)
			state[i][j] ^= key[i + 4 * j];
}

std::vector<uint8_t> AES::XorColumns(const std::vector<uint8_t>& first,
	const std::vector<uint8_t>& second, const std::vector<uint8_t>& third)
{
	std::vector<uint8_t> res(4);
	res[0] = first[0] ^ second[0] ^ third[0];
	res[1] = first[1] ^ second[1] ^ third[1];
	res[2] = first[2] ^ second[2] ^ third[2];
	res[3] = first[3] ^ second[3] ^ third[3];
	return res;
}

void AES::LeftShiftColumn(std::vector<uint8_t>& column)
{
	uint8_t t = column[0];
	column[0] = column[1];
	column[1] = column[2];
	column[2] = column[3];
	column[3] = t;
}

void AES::SubColumn(std::vector<uint8_t>& column)
{
	for (int i = 0; i < 4; i++)
		column[i] = sBox[column[i]];
}

std::vector<uint8_t> AES::GetRConColumn(const int& index)
{
	std::vector<uint8_t> res(4);
	for (int i = 0; i < 4; ++i)
		res[i] = RCon[10 * i + index];
	return res;
}

/// <summary>
/// Multiplicataion by 0x02
/// </summary>
/// <param name="value"></param>
/// <returns></returns>
uint8_t AES::xTime(const uint8_t& value)
{
	return (value << 1) ^ (((value >> 7) & 1) * 0x1b);
}

uint8_t AES::MultBy(const uint8_t& input, const uint8_t& value)
{
	uint8_t mult = value;
	std::vector<uint8_t> forXor;
	while (true)
	{
		uint8_t s = uint8_t(0x01);
		int iter = 0;
		while (uint8_t((s << 1)) <= uint8_t(mult))
		{
			++iter;
			s = s << 1;
		}

		uint8_t xorValue = uint8_t(input);
		for (int i = 0; i < iter; ++i)
			xorValue = xTime(xorValue);
		forXor.push_back(xorValue);

		uint8_t diff = mult - s;
		if (diff == 0x01 || diff == 0x00)
		{
			forXor.push_back(diff * input);
			break;
		}

		mult = diff;
	}

	uint8_t res = 0x00;
	for (uint8_t& val : forXor)
		res ^= val;
	return res;
}

void AES::PushPaddingZeros(std::vector<uint8_t>& msg)
{
	int steps = 16 * ceil(msg.size() / 16.0) - msg.size();
	for (int i = 0; i < steps; ++i)
		msg.push_back(0x30);
}

std::vector<uint8_t> AES::EncryptBlock(const std::vector<uint8_t>& input,
	const std::vector<std::vector<uint8_t>>& roundKeys)
{
	std::vector<std::vector<uint8_t>> state(4, std::vector<uint8_t>(Nb));
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < Nb; ++j)
			state[i][j] = input[i + 4 * j];

	AddRoundKey(state, GetKeyFromRoundKeys(roundKeys, 0));

	for (int round = 1; round < Nr; ++round)
	{
		SubBytes(state);
		ShiftRows(state);
		MixColumns(state);
		AddRoundKey(state, GetKeyFromRoundKeys(roundKeys, round));
	}

	SubBytes(state);
	ShiftRows(state);
	AddRoundKey(state, GetKeyFromRoundKeys(roundKeys, Nr));

	std::vector<uint8_t> res(4 * Nb);
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < Nb; ++j)
			res[i + j * 4] = state[i][j];

	return res;
}

std::vector<uint8_t> AES::DecryptBlock(const std::vector<uint8_t>& input,
	const std::vector<std::vector<uint8_t>>& roundKeys)
{
	std::vector<std::vector<uint8_t>> state(4, std::vector<uint8_t>(Nb));
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < Nb; ++j)
			state[i][j] = input[i + 4 * j];

	AddRoundKey(state, GetKeyFromRoundKeys(roundKeys, Nr));

	for (int round = Nr - 1; round > 0; --round)
	{
		InvShiftRows(state);
		InvSubBytes(state);
		AddRoundKey(state, GetKeyFromRoundKeys(roundKeys, round));
		InvMixColumns(state);
	}

	InvShiftRows(state);
	InvSubBytes(state);
	AddRoundKey(state, GetKeyFromRoundKeys(roundKeys, 0));

	std::vector<uint8_t> res(4 * Nb);
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < Nb; ++j)
			res[i + j * 4] = state[i][j];

	return res;
}

std::vector<uint8_t> AES::GetKeyFromRoundKeys(const std::vector<std::vector<uint8_t>>& rKeys, const int& blockIndex)
{
	std::vector<uint8_t> res(4 * Nb);
	for (int i = 0; i < 4; ++i)
	{
		res[0 + i * 4] = rKeys[0][blockIndex * Nb + i];
		res[1 + i * 4] = rKeys[1][blockIndex * Nb + i];
		res[2 + i * 4] = rKeys[2][blockIndex * Nb + i];
		res[3 + i * 4] = rKeys[3][blockIndex * Nb + i];
	}
	return res;
}

void AES::Encrypt(std::string inputFilePath, std::string outputFilePath)
{
	std::ifstream inputFile(inputFilePath);
	if (!inputFile.is_open())
		throw "Something went wrong while opening input file.";

	std::ofstream outputFile(outputFilePath);
	if(!outputFile.is_open())
		throw "Something went wrong while opening output file.";

	inputFile.seekg(0, inputFile.end);
	long long fileLength = inputFile.tellg();
	inputFile.seekg(0, inputFile.beg);

	for (int i = 0; i < fileLength; i+=blockBytesLen)
	{
		std::vector<uint8_t> input(blockBytesLen);
		inputFile.read(reinterpret_cast<char*>(input.data()), blockBytesLen);

		auto tmp = EncryptBlock(input, m_roundKeys);
		outputFile.write(reinterpret_cast<char*>(tmp.data()), tmp.size());
	}
	
	inputFile.close();
	outputFile.close();
}

void AES::Decrypt(std::string inputFilePath, std::string outputFilePath)
{
	std::ifstream inputFile(inputFilePath);
	if (!inputFile.is_open())
		throw "Something went wrong while opening input file.";

	std::ofstream outputFile(outputFilePath);
	if (!outputFile.is_open())
		throw "Something went wrong while opening output file.";

	inputFile.seekg(0, inputFile.end);
	long long fileLength = inputFile.tellg();
	inputFile.seekg(0, inputFile.beg);

	for (int i = 0; i < fileLength; i += blockBytesLen)
	{
		std::vector<uint8_t> input(blockBytesLen);
		inputFile.read(reinterpret_cast<char*>(input.data()), blockBytesLen);

		auto tmp = DecryptBlock(input, m_roundKeys);
		outputFile.write(reinterpret_cast<char*>(tmp.data()), tmp.size());
	}
	inputFile.close();
	outputFile.close();
}