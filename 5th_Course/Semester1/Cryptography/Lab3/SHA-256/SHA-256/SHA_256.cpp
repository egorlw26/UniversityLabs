#include "SHA_256.h"
#include <fstream>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>

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

int SHA_256::calcPadding()
{
	int k = 0;
	while ((m_message_length_in_bits + 1 + k) % 512 != 448) ++k;
	return k;
}

void SHA_256::stringToBytes(const std::string& msg)
{
	auto start = reinterpret_cast<const unsigned char*>(msg.c_str());
	m_bytes.assign(start, start + msg.length());
	m_message_length_in_bits = msg.length() * 8;
}

void SHA_256::padding()
{
	int padding = calcPadding();

	// pushing symbol after end of input message
	m_bytes.push_back(0x80);
	padding -= 7;

	for (int i = 0; i < (padding / 8); ++i)
		m_bytes.push_back(0);

	for (int i = 1; i < 9; ++i)
		m_bytes.push_back(m_message_length_in_bits >> (64 - i * 8));
}

void SHA_256::parse()
{
	unsigned int iter = 0;
	for (; iter < m_bytes.size() / 64; ++iter)
	{
		// chunk of 512-bits
		std::vector<unsigned int> block(16);
		for (int j = 0; j < 16; ++j)
		{
			unsigned int word = 0;
			for (int k = 0; k < 4; ++k)
			{
				word <<= 8;
				word |= m_bytes[iter*64 + j * 4 + k];
			}
			block[j] = word;
		}
		m_blocks.push_back(block);
	}
	m_numberOfBlocks = iter;
}

void SHA_256::computeHash()
{
	// Initialize hash values (8 numbers)
	m_hash = 
	{ 
		0x6a09e667,
		0xbb67ae85,
		0x3c6ef372,
		0xa54ff53a,
		0x510e527f,
		0x9b05688c,
		0x1f83d9ab,
		0x5be0cd19
	};
	// schedule array of 32-bits words
	auto w = std::vector<unsigned int>(64, 0);
	// variables for round
	unsigned int a, b, c, d, e, f, g, h;
	// temp variables
	unsigned int temp1, temp2;
	for (int i = 0; i < m_numberOfBlocks; ++i)
	{
		for (int t = 0; t < 16; ++t)
			w[t] = m_blocks[i][t];
		for (int t = 16; t < 64; ++t)
			w[t] = sSigma1(w[t - 2]) + w[t - 7] + sSigma0(w[t - 15]) + w[t - 16];

		a = m_hash[0];
		b = m_hash[1];
		c = m_hash[2];
		d = m_hash[3];
		e = m_hash[4];
		f = m_hash[5];
		g = m_hash[6];
		h = m_hash[7];

		for (int t = 0; t < 64; ++t)
		{
			temp1 = h + lSigma1(e) + Ch(e, f, g) + K[t] + w[t];
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

		m_hash[0] += a;
		m_hash[1] += b;
		m_hash[2] += c;
		m_hash[3] += d;
		m_hash[4] += e;
		m_hash[5] += f;
		m_hash[6] += g;
		m_hash[7] += h;
	}
}

void SHA_256::showResultHash()
{
	std::cout << getResultString() << std::endl;
}

std::string SHA_256::getResultString()
{
	std::stringstream ss;
	for (int i = 0; i < 8; ++i)
		ss << std::hex << std::setw(8) << std::setfill('0') << m_hash[i];
	return ss.str();
}

void SHA_256::clearState()
{
	m_blocks.clear();
	m_bytes.clear();
}

void SHA_256::createHash(const std::string& i_msg)
{
	stringToBytes(i_msg);
	padding();
	parse();
	computeHash();
	clearState();
}

SHA_256::SHA_256(const std::string& msg)
{
	createHash(msg);
}