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

	unsigned long long iterator = 0;
	std::set<std::string> resultsForSearch;

	std::string testString;
	std::string initialText = std::to_string(rand() % RAND_MAX);

	while (true)
	{
		testString = initialText + std::to_string(iterator);

		auto resHash = !SHA_or_KUPYNA
			? SHA_256(testString).getResultString()
			: Kupyna(kupynaSize, testString).getResultString();
		if (resultsForSearch.find(resHash.substr(0, substrLengthReq)) != resultsForSearch.end())
		{
			/*std::cout << "Partial collision for length " << substrLengthReq << " found at " << iterator << " iteration\n"
				<< "First hash: '" << *resultsForSearch.find(resHash.substr(0, substrLengthReq)) << "'\n"
				<< "Second hash: '" << resHash << "'\n";*/
			break;
		}
		else
		{
			resultsForSearch.insert(resHash.substr(0, substrLengthReq));
		}
		iterator++;
	}
	return iterator;
}

void calculateAverageIterationsAndTime(int substrLengthReq, bool SHA_or_KUPYNA, int kupynaSize = 256)
{
	const int iterations = 200;
	double avgIters = 0.0;

	auto start = std::chrono::steady_clock::now();

	for (int i = 0; i < iterations; ++i)
	{
		avgIters += (double)(partialCollisions(substrLengthReq, SHA_or_KUPYNA, kupynaSize)) / iterations;
	}

	auto avgTime = (double)(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count()) / iterations;

	auto typeStr = !SHA_or_KUPYNA
		? "SHA_256"
		: kupynaSize == 256 ? "Kupyna-256" : "Kupyna-512";

	std::cout << "For " << typeStr << " and length " << substrLengthReq << " took " << avgIters << " iterations and " << avgTime << " milliseconds in average\n";
}

int main()
{
	srand(time(NULL));
	for (int i = 1; i <= 7; ++i)
		calculateAverageIterationsAndTime(i, 1, 512);
	return 0;
}