#include <iostream>
#include "RSA.h"

int main()
{
	for (auto bitLength : {16, 32, 64, 128, 256 })
	{
		srand(time(NULL));
		std::cout << std::endl << std::endl;
		RSA rsa(bitLength);
		BigInt message = 3456;
		auto encrypted = rsa.Encrypt(message);
		std::cout << "Encrypted: " << encrypted << "\n";
		auto decrypted = rsa.Decrypt(encrypted);
		std::cout << "Decrypted: " << decrypted << "\n";
	}

	/*BigInt tooBigInt = AdditionalFuncs::findRandomNearestPrime(4);*/
}