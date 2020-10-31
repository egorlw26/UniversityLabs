#include "../../SHA-256/SHA-256/SHA_256.h"
#include "../../SHA-256/SHA-256/SHA_256.cpp"
#include "../../Kupyna/Kupyna/Kupyna/Kupyna.h"
#include "../../Kupyna/Kupyna/Kupyna/Kupyna.cpp"
#include <iostream>
#include <string>

void partialCollisions(bool SHA_or_KUPYNA /* 0 - SHA, 1 - Kupyna*/)
{
	const std::string originalString = "abcd";
	const std::string hashOriginal = SHA_or_KUPYNA ? SHA_256(originalString).getResultString() : Kupyna(256, originalString).getResultString();
	unsigned long long iterator = 0;
	std::string testString = "";
	int testStringLength = 1;

	int substrLengthReq = 6;

	while (true)
	{
		if (iterator > 0 && iterator % 100000 == 0)
		{
			testStringLength++;
		}

		/*if (iterator > 0 && iterator % 1000 == 0)
		{
			std::cout << "Iteration: " << iterator << ", length: " << testStringLength << std::endl;
		}*/

		testString = "";
		for (int i = 0; i < testStringLength; ++i)
			testString += (char)(rand() % 256);

		if (testString != originalString)
		{
			auto resHash = SHA_or_KUPYNA ? SHA_256(testString).getResultString() : Kupyna(256, testString).getResultString();
			if (resHash.substr(0, substrLengthReq) == hashOriginal.substr(0, substrLengthReq))
			{
				std::cout << "Partial collision for length " << substrLengthReq << " found at " << iterator << " iteration\n";
				break;
			}
		}
		iterator++;
	}
}

int main()
{
	partialCollisions(0);
	return 0;
}