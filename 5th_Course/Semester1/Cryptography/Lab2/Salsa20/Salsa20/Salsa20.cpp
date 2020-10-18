#include "Salsa20.h"

#include <chrono>
#include <fstream>
#include <iostream>

Salsa20::Salsa20(std::string key):
    m_key(key)
{
    m_init_key = std::vector<uint32_t>(16, 0);
    ExpandKey();
}

uint32_t Salsa20::Rotate(uint32_t value, uint8_t shift)
{
	return (value << shift) | (value >> (32 - shift));
}

void Salsa20::QR(uint32_t& y0, uint32_t& y1, uint32_t& y2, uint32_t& y3)
{
    y1 ^= Rotate((y0 + y3), 7);
    y2 ^= Rotate((y1 + y0), 9);
    y3 ^= Rotate((y2 + y1), 13);
    y0 ^= Rotate((y3 + y2), 18);
}

void Salsa20::SalsaBlock(uint32_t* state, size_t state_size, size_t stream_pos)
{
    uint32_t x[16];
    std::copy(m_init_key.begin(), m_init_key.end(), x);
    x[8] = (stream_pos % std::numeric_limits<uint32_t>::max());
    x[9] = (stream_pos / std::numeric_limits<uint32_t>::max());

    for (size_t i = 0; i < 10; ++i)
    {
        QR(x[0], x[4], x[8], x[12]);
        QR(x[5], x[9], x[13], x[1]);
        QR(x[10], x[14], x[2], x[6]);
        QR(x[15], x[3], x[7], x[11]);

        QR(x[0], x[1], x[2], x[3]);
        QR(x[5], x[6], x[7], x[4]);
        QR(x[10], x[11], x[8], x[9]);
        QR(x[15], x[12], x[13], x[14]);
    }
    for (size_t i = 0; i < 16; ++i)
        x[i] += m_init_key[i];

    for (size_t i = 0; i < state_size; ++i)
        state[i] ^= x[i];
}

std::string Salsa20::CryptString(const std::string& input)
{
    std::string res(input);
    auto inputInWords = reinterpret_cast<uint32_t*>(res.data());
    const auto numOfWords = (res.size() + 3) / 4;
    const auto numOfBlocks = (numOfWords + 15) / 16;
    for (int i = 0; i < numOfBlocks; ++i)
        SalsaBlock(inputInWords, 16, i);
    return res;
}

void Salsa20::ExpandKey()
{
    auto temp_key = m_key;
    const auto keyAsUint8 = reinterpret_cast<const uint8_t*>(temp_key.data());
    auto wordsToBytes = reinterpret_cast<uint8_t*>(m_init_key.data());

    std::copy(&m_key32[0], &m_key32[4], &wordsToBytes[0]);
    std::copy(keyAsUint8, keyAsUint8 + 16, &wordsToBytes[4]);
    std::copy(&m_key32[4], &m_key32[8], &wordsToBytes[20]);
    std::copy(&m_key32[8], &m_key32[12], &wordsToBytes[40]);
    std::copy(keyAsUint8 + 16, keyAsUint8 + 32, &wordsToBytes[44]);
    std::copy(&m_key32[12], &m_key32[16], &wordsToBytes[60]);
}

void Salsa20::Encrypt(const std::string inputFilePath, const std::string outputFilePath)
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

    auto startTime = std::chrono::system_clock::now();

    int maxSize = 64;
    std::string message;
    message.resize(maxSize);

    for (int i = 0; i < fileLength; i += maxSize)
    {
        inputFile.read(message.data(), maxSize);
        auto encrypted = CryptString(message);
        outputFile.write(encrypted.data(), maxSize);
    }

    std::cout << "Encrypted at " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count()
        << " milliseconds" << std::endl;

    inputFile.close();
    outputFile.close();
}

void Salsa20::Decrypt(const std::string inputFilePath, const std::string outputFilePath)
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

    auto startTime = std::chrono::system_clock::now();

    int maxSize = 64;
    std::string message;
    message.resize(maxSize);

    for (int i = 0; i < fileLength; i += maxSize)
    {
        inputFile.read(message.data(), maxSize);
        auto encrypted = CryptString(message);
        outputFile.write(encrypted.data(), maxSize);
    }

    std::cout << "Decrypted at " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count()
        << " milliseconds" << std::endl;

    inputFile.close();
    outputFile.close();
}