#pragma once
#include "bigint.h"
#include "additional.h"

class RSA
{
private:
	BigInt _p;
	BigInt _q;
	BigInt _n;
	BigInt _totient;
	BigInt _e;
	BigInt _d;
public:
	RSA();
	BigInt Encrypt(const BigInt& message) const;
	BigInt Decrypt(const BigInt& input) const;
};

