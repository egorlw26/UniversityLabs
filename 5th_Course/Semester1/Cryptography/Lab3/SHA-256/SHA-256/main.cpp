#include "SHA_256.h"


int main()
{
	// abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq

	std::string msgToHash = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";

	SHA_256 sha = SHA_256(msgToHash);
	
	sha.showResultHash();

	return 0;
}