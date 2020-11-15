#include "RSA.h"
#include <iostream>

RSA::RSA()
{
	_p = AdditionalFuncs::findRandomNearestPrime(16);
	std::cout << "Found 'p' " << _p << " as random prime\n";
	_q = AdditionalFuncs::findRandomNearestPrime(16);
	std::cout << "Found 'q' " << _q << " as random prime\n";
	_n = _p * _q;
	std::cout << "'n' calculated " << _n << "\n";
	_totient = (_p - 1) * (_q - 1);
	std::cout << "Totient " << _totient << " calculated\n";

	for (_e = 3; _e < _totient; _e = _e + 1)
	{
		if (_e.bi_gcd(_totient) == 1)
		{
			break;
		}
	}
	std::cout << "'e' " << _e << " found\n";

	_d = AdditionalFuncs::ExtendedEuclidianAlgo(_e, _totient);
	std::cout << "'d' " << _d <<" found\n";
}

BigInt RSA::Encrypt(const BigInt& input) const
{
	if (input.getBIBitLength() > _n)
	{
		throw "Message too long for encrypting, abort";
	}

	std::cout << "\n\nStarting encrypting\n\n";
	// Encryption func is c = m^e mod n, where m - our message
	return input.powerMod(_e, _n);
}

BigInt RSA::Decrypt(const BigInt& input) const
{
	std::cout << "\n\nStarting decrypting\n\n";
	// Decryption func is m = c^d mod n, where:
	// c - input encrypted message, m - output decrypted message
	return input.powerMod(_d, _n);
}
