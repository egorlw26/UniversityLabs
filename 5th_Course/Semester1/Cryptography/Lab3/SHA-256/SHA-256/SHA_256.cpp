#include "SHA_256.h"
#include <fstream>
#include <chrono>
#include <iostream>
#include <iomanip>

SHA_256::SHA_256()
{
	m_W = std::vector<unsigned int>(64);
}


unsigned int SHA_256::ROTR(const unsigned int& n, const unsigned int& x)
{
	return (x >> n) | (x << (32 - n));
}

unsigned int SHA_256::SHR(const unsigned int& n, const unsigned int& x)
{
	return x >> n;
}

unsigned int SHA_256::Ch(const unsigned int& x, const unsigned int& y, const unsigned int& z)
{
	return (x & y) ^ (~x & z);
}

unsigned int SHA_256::Maj(const unsigned int& x, const unsigned int& y, const unsigned int& z)
{
	return (x & y) ^ (x & z) ^ (y & z);
}

unsigned int SHA_256::lSigma0(const unsigned int& x)
{
	return ROTR(2, x) ^ ROTR(13, x) ^ ROTR(22, x);
}

unsigned int SHA_256::lSigma1(const unsigned int& x)
{
	return ROTR(6, x) ^ ROTR(11, x) ^ ROTR(25, x);
}

unsigned int SHA_256::sSigma0(const unsigned int& x)
{
	return ROTR(7, x) ^ ROTR(18, x) ^ SHR(3, x);
}

unsigned int SHA_256::sSigma1(const unsigned int& x)
{
	return ROTR(17, x) ^ ROTR(19, x) ^ SHR(10, x);
}

void SHA_256::initHash()
{
	m_H.push_back({ 0x6a09e667,
					0xbb67ae85,
					0x3c6ef372,
					0xa54ff53a,
					0x510e527f,
					0x9b05688c,
					0x1f83d9ab,
					0x5be0cd19 });
}

int SHA_256::calcPadding()
{
	int k = 0;
	int length = m_bytes.size() * 8;
	while ((length + 1 + k) % 512 != 448) ++k;
	return k;
}

void SHA_256::stringToBytes(const std::string& msg)
{
	auto start = reinterpret_cast<const unsigned char*>(msg.c_str());
	m_bytes.assign(start, start + msg.length());
}

void SHA_256::paddingBytes()
{
	auto padding = calcPadding();
	auto length = m_bytes.size()*8;

	m_bytes.push_back(0x80);
	padding -= 7;

	for (int i = 0; i < (padding / 8); ++i)
	{
		m_bytes.push_back(0);
	}

	for (int i = 1; i < 9; ++i)
	{
		m_bytes.push_back(length >> (64 - i * 8));
	}
}

void SHA_256::parseBytes()
{
	unsigned int iter = 0;
	for (; iter < m_bytes.size() / 64; ++iter)
	{
		std::vector<unsigned int> block(16);
		for (int j = 0; j < 16; ++j)
		{
			unsigned int wordInBlock = 0;
			for (int k = 0; k < 4; ++k)
			{
				wordInBlock <<= 8;
				wordInBlock |= m_bytes[j * 4 + k];
			}
			block[j] = wordInBlock;
		}
		m_M.push_back(block);
	}
	m_numberOfBlocks = iter;
}

void SHA_256::computeHash()
{
	std::vector<unsigned int> hashBlock(8);
	for (int i = 1; i <= m_numberOfBlocks; ++i)
	{
		for (int t = 0; t < 16; ++t)
			m_W[t] = m_M[i - 1][t];
		for (int t = 16; t <= 63; ++t)
			m_W[t] = sSigma1(m_W[t - 2]) + m_W[t - 7] + sSigma0(m_W[t - 15]) + m_W[t - 16];

		unsigned int a, b, c, d, e, f, g, h;
		a = m_H[i - 1][0];
		b = m_H[i - 1][1];
		c = m_H[i - 1][2];
		d = m_H[i - 1][3];
		e = m_H[i - 1][4];
		f = m_H[i - 1][5];
		g = m_H[i - 1][6];
		h = m_H[i - 1][7];

		unsigned int temp1, temp2;

		for (int t = 0; t <= 63; ++t)
		{
			temp1 = h + lSigma1(e) + Ch(e, f, g) + K[t] + m_W[t];
			temp2 = lSigma0(a) + Maj(a, b, c);

			h = g;
			g = f;
			f = e;
			e = d + temp1;
			d = c;
			c = b;
			b = a;
			a = temp1 + temp2;
		}

		hashBlock[0] = a + m_H[i - 1][0];
		hashBlock[1] = b + m_H[i - 1][1];
		hashBlock[2] = c + m_H[i - 1][2];
		hashBlock[3] = d + m_H[i - 1][3];
		hashBlock[4] = e + m_H[i - 1][4];
		hashBlock[5] = f + m_H[i - 1][5];
		hashBlock[6] = g + m_H[i - 1][6];
		hashBlock[7] = h + m_H[i - 1][7];

		m_H.push_back(hashBlock);
	}
}

void SHA_256::showResultHash()
{
	for (int i = 0; i < 8; ++i)
		std::cout << std::hex << std::setw(8) << std::setfill('0') << m_H[m_numberOfBlocks][i];
	std::cout << std::endl;

	clearState();
}

void SHA_256::clearState()
{
	m_M.clear();
	m_H.clear();
	m_bytes.clear();
}

void SHA_256::createHash(const std::string& i_msg)
{
	stringToBytes(i_msg);
	paddingBytes();
	parseBytes();
	initHash();
	computeHash();
}
