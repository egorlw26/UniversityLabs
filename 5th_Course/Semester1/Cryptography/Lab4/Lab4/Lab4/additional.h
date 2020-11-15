#pragma once
#include "bigint.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <math.h>
#include <map>
#include <chrono>

namespace AdditionalFuncs
{
	bool MillerRabin(const BigInt& n, const BigInt& k);
	BigInt rand(const BigInt& N);
	BigInt gxRho(const BigInt& x, const BigInt& mod);
	BigInt RhoPollandF(const BigInt& N);
	std::pair<BigInt, BigInt> whatPower(const BigInt& number);
	std::vector<std::pair<BigInt, BigInt>> Factorize(const BigInt& number);
	BigInt EulerFunc(const BigInt& n);
	int MobiusFunc(const BigInt& n);
	BigInt ExtendedEuclidianAlgo(const BigInt& e, const BigInt& n);
	BigInt randomPrime();
	BigInt findRandomNearestPrime(const int bitLength);
	BigInt findPrimitiveRootModule(const BigInt& number);
}