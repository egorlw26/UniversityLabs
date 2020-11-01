#include <../../SHA-256/SHA-256/SHA_256.h>
#include <../../SHA-256/SHA-256/SHA_256.cpp>
#include <../../Kupyna/Kupyna/Kupyna.h>
#include <../../Kupyna/Kupyna/Kupyna.cpp>
#include <iostream>
#include <string>
#include <set>
#include <time.h>
#include <chrono>

int partialCollisions(int substrLengthReq, bool SHA_or_KUPYNA /* 0 - SHA, 1 - Kupyna*/, int kupynaSize = 256)
{
	const std::string originalString = "abcd";
	const std::string hashOriginal = SHA_or_KUPYNA ? SHA_256(originalString).getResultString() : Kupyna(256, originalString).getResultString();
	unsigned long long iterator = 0;
	std::string testString(1024, '0');
	std::set<std::string> resultsForSearch{hashOriginal.substr(0, substrLengthReq)};

	while (true)
	{
		auto index = rand() % testString.length();
		testString[index] = rand() % 256;

		if (testString != originalString)
		{
			auto resHashSubstr = SHA_or_KUPYNA ? SHA_256(testString).getResultString() : Kupyna(256, testString).getResultString().substr(0, substrLengthReq);
			if (resultsForSearch.find(resHashSubstr) != resultsForSearch.end())
			{
				//std::cout << "Partial collision for length " << substrLengthReq << " found at " << iterator << " iteration\n";
				break;
			}
			else
			{
				resultsForSearch.insert(resHashSubstr);
			}
		}
		iterator++;
	}
	return iterator;
}

void calculateAverageIterationsAndTime(int substrLengthReq, bool SHA_or_KUPYNA, int kupynaSize = 256)
{
	const int iterations = 1000;
	double avgIters = 0.0;

	auto start = std::chrono::steady_clock::now();

	for (int i = 0; i < iterations; ++i)
	{
		avgIters += (double)(partialCollisions(substrLengthReq, SHA_or_KUPYNA, kupynaSize)) / iterations;
	}

	auto avgTime = (double)(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count()) / iterations;

	auto typeStr = SHA_or_KUPYNA ? "SHA_256" : kupynaSize == 256 ? "Kupyna-256" : "Kupyna-512";

	std::cout << "For " << typeStr << " and length " << substrLengthReq << " took " << avgIters << " iterations and " << avgTime << " milliseconds in average\n";
}

int main()
{
	srand(time(NULL));
	calculateAverageIterationsAndTime(1, 0);
	return 0;
}