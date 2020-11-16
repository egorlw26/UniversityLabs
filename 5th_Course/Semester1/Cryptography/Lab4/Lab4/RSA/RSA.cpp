#include "RSA.h"
#include "BigIntMod.h"
#include <chrono>
#include <time.h>
#include <iostream>

RSA::RSA(const int bitLength)
{
	auto start = std::chrono::steady_clock::now();

	_p = AdditionalFuncs::findRandomNearestPrime(bitLength);
	std::cout << "Found 'p' " << _p << " as random prime\n";
	_q = AdditionalFuncs::findRandomNearestPrime(bitLength-1);
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

	auto end = std::chrono::steady_clock::now();
	auto timeTook = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "Init time took: " << timeTook << " milliseconds\n";
}

BigInt RSA::Encrypt(const BigInt& input) const
{
	auto start = std::chrono::steady_clock::now();

	if (input.getBIBitLength() > _n)
	{
		throw "Message too long for encrypting, abort";
	}

	std::cout << "\n\nStarting encrypting\n\n";
	// Encryption func is c = m^e mod n, where m - our message

	auto res = input.powerMod(_e, _n);

	auto end = std::chrono::steady_clock::now();
	auto timeTook = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "Encryption time took: " << timeTook << " milliseconds\n";

	return res;
}

BigInt RSA::Decrypt(const BigInt& input) const
{
	auto start = std::chrono::steady_clock::now();

	std::cout << "\n\nStarting decrypting\n\n";
	// Decryption func is m = c^d mod n, where:
	// c - input encrypted message, m - output decrypted message

	// Using chinese theory

	BigInt dp = _d.bi_mod(_p - 1);
	BigInt dq = _d.bi_mod(_q - 1);
	BigInt qInv = BigIntMod(_q, _p).inverse().getNumber();

	auto m1 = input.powerMod(dp, _p);
	auto m2 = input.powerMod(dq, _q);
	auto h = (qInv * (m1 - m2)) % _p;
	auto m = (m2 + h * _q) % _n;

	auto end = std::chrono::steady_clock::now();
	auto timeTook = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "Decryption time took: " << timeTook << " milliseconds\n";

	return m;
}
