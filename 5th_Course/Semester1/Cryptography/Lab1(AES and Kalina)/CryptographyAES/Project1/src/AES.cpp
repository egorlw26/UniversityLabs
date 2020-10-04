#include "AES.h"
#include <fstream>
#include <chrono>

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

	m_roundKeys = std::vector<std::vector<uint8_t>>(4, std::vector<uint8_t>(Nb * (Nr + 1)));
	KeyExpansion(key, m_roundKeys);
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

void AES::ShiftRow(std::vector<uint8_t>& row, const int step)
{
	auto row_copy = row;
	for (int i = 0; i < Nb; ++i)
		row[i] = row_copy[(i + step) % Nb];
}

void AES::MixColumns(std::vector<std::vector<uint8_t>>& state)
{
	for (int i = 0; i < 4; ++i)
	{
		uint8_t a0 = state[0][i];
		uint8_t a1 = state[1][i];
		uint8_t a2 = state[2][i];
		uint8_t a3 = state[3][i];

		state[0][i] = xTime(a0) ^ MultBy(a1, 0x03) ^ a2 ^ a3;
		state[1][i] = a0 ^ xTime(a1) ^ MultBy(a2, 0x03) ^ a3;
		state[2][i] = a0 ^ a1 ^ xTime(a2) ^ MultBy(a3, 0x03);
		state[3][i] = MultBy(a0, 0x03) ^ a1 ^ a2 ^ xTime(a3);
	}
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

void AES::InvShiftRow(std::vector<uint8_t>& row, const int step)
{
	auto row_copy = row;
	for (int i = 0; i < 4; ++i)
	{
		row[i] = row_copy[((i - step) % Nb + Nb) % Nb];
	}
}

void AES::InvMixColumns(std::vector<std::vector<uint8_t>>& state)
{
	for (int i = 0; i < 4; ++i)
	{
		uint8_t a0 = state[0][i];
		uint8_t a1 = state[1][i];
		uint8_t a2 = state[2][i];
		uint8_t a3 = state[3][i];

		state[0][i] = MultBy(a0, 0x0e) ^ MultBy(a1, 0x0b) ^
			MultBy(a2, 0x0d) ^ MultBy(a3, 0x09);
		state[1][i] = MultBy(a0, 0x09) ^ MultBy(a1, 0x0e) ^
			MultBy(a2, 0x0b) ^ MultBy(a3, 0x0d);
		state[2][i] = MultBy(a0, 0x0d) ^ MultBy(a1, 0x09) ^
			MultBy(a2, 0x0e) ^ MultBy(a3, 0x0b);
		state[3][i] = MultBy(a0, 0x0b) ^ MultBy(a1, 0x0d) ^
			MultBy(a2, 0x09) ^ MultBy(a3, 0x0e);
	}
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
			XorColumns(nColumn, Wi_m_Nk, GetRConColumn(i / Nk - 1));
		}
		else
			XorColumns(nColumn, Wi_m_Nk, { 0x00, 0x00, 0x00, 0x00 });

		w[0][i] = nColumn[0];
		w[1][i] = nColumn[1];
		w[2][i] = nColumn[2];
		w[3][i] = nColumn[3];
	}
}

void AES::AddRoundKey(std::vector<std::vector<uint8_t>>& state, const int round)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < Nb; ++j)
			state[i][j] ^= m_roundKeys[j][round * Nb + i];
}

void AES::XorColumns(std::vector<uint8_t>& first,
	const std::vector<uint8_t>& second, const std::vector<uint8_t>& third)
{
	for(int i = 0; i < 4; ++i)
		first[i] = first[i] ^ second[i] ^ third[i];
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

std::vector<uint8_t> AES::GetRConColumn(const int index)
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
uint8_t AES::xTime(const uint8_t value)
{
	return (value << 1) ^ (((value >> 7) & 1) * 0x1b);
}

uint8_t AES::MultBy(const uint8_t input, uint8_t value)
{
	uint8_t res = 0x00;
	while (true)
	{
		uint8_t s = 0x01;
		int iter = 0;
		while ((s << 1) <= value)
		{
			++iter;
			s = s << 1;
		}
		uint8_t xorValue = input;
		for (int i = 0; i < iter; ++i)
			xorValue = xTime(xorValue);
		res ^= xorValue;

		uint8_t diff = value - s;
		if (diff == 0x01 || diff == 0x00)
		{
			res ^= (diff * input);
			break;
		}

		value = diff;
	}
	return res;
}

void AES::PushPaddingZeros(std::vector<uint8_t>& msg)
{
	int steps = 16 * ceil(msg.size() / 16.0) - msg.size();
	for (int i = 0; i < steps; ++i)
		msg.push_back(0x30);
}

void AES::EncryptBlock(std::vector<std::vector<uint8_t>>& input)
{
	AddRoundKey(input, 0);

	for (int round = 1; round < Nr; ++round)
	{
		SubBytes(input);
		ShiftRows(input);
		MixColumns(input);
		AddRoundKey(input, round);
	}

	SubBytes(input);
	ShiftRows(input);
	AddRoundKey(input, Nr);
}

void AES::DecryptBlock(std::vector<std::vector<uint8_t>>& input)
{
	AddRoundKey(input, Nr);

	for (int round = Nr - 1; round > 0; --round)
	{
		InvShiftRows(input);
		InvSubBytes(input);
		AddRoundKey(input, round);
		InvMixColumns(input);
	}

	InvShiftRows(input);
	InvSubBytes(input);
	AddRoundKey(input, 0);
}

void AES::Encrypt(std::string inputFilePath, std::string outputFilePath)
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

	std::vector<std::vector<uint8_t>> input(4, std::vector<uint8_t>(Nb));

	auto startTime = std::chrono::system_clock::now();

	for (int i = 0; i < fileLength; i += blockBytesLen)
	{
		for(int j = 0; j < Nb; ++j)
			inputFile.read(reinterpret_cast<char*>(input[j].data()), 4);

		EncryptBlock(input);

		for(int j = 0; j < Nb; ++j)
			outputFile.write(reinterpret_cast<char*>(input[j].data()), input[j].size());
	}

	std::cout << "Encrypted at " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count()
		<< " milliseconds" << std::endl;

	inputFile.close();
	outputFile.close();
}

void AES::Decrypt(std::string inputFilePath, std::string outputFilePath)
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

	std::vector<std::vector<uint8_t>> input(4, std::vector<uint8_t>(4));

	auto startTime = std::chrono::system_clock::now();

	for (int i = 0; i < fileLength; i += blockBytesLen)
	{
		for(int j = 0; j < Nb; ++j)
			inputFile.read(reinterpret_cast<char*>(input[j].data()), 4);
		DecryptBlock(input);

		for(int j = 0; j < Nb; ++j)
			outputFile.write(reinterpret_cast<char*>(input[j].data()), input[j].size());
	}

	std::cout << "Decrypted at " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count()
		<< " milliseconds" << std::endl;

	inputFile.close();
	outputFile.close();
}