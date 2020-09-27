#include "AES.h"

AES::AES()
{
	// Length of key is 128 bit
	this->Nb = 4;
	this->Nk = 4;
	this->Nr = 10;

	blockBytesLen = 4 * this->Nb * sizeof(unsigned char);
};

void AES::SubBytes(std::vector<std::vector<uint8_t>>& state)
{
	for (int i =0; i < 4; ++i)
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
	column[2] = MultBy(a[0], 0x03) ^ a[1] ^ a[2] ^ xTime(a[3]);
}

void AES::KeyExpansion(const std::vector<uint8_t>& key, std::vector<std::vector<uint8_t>>& w)
{
	//w = std::vector<std::vector<uint8_t>>(4, std::vector<uint8_t>(Nb * (Nr + 1)));

	// First Round key
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
		for (int j = 0; j < 4; ++j)
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
		res[i] = RCon[10*i + index];
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
	for (int i = 0; i < msg.size() - 4 * Nb; ++i)
		msg.push_back(0x00);
}

std::vector<uint8_t> AES::EncryptBlock(const std::vector<uint8_t>& input,
	const std::vector<std::vector<uint8_t>>& roundKeys)
{
	std::vector<std::vector<uint8_t>> state(4, std::vector<uint8_t>(Nb));
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < Nb; ++j)
			state[i][j] = input[i + 4 * j];

	// Finish
}

std::vector<uint8_t> AES::Encrypt(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key)
{
	// If input is less than 16 bytes
	auto inputCopy = input;
	PushPaddingZeros(inputCopy);
	
	std::vector<std::vector<uint8_t>> roundKeys(4, std::vector<uint8_t>(Nb * (Nr + 1)));
	KeyExpansion(key, roundKeys);

}