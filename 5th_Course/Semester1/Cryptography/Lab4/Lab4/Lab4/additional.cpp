#include "additional.h"

BigInt AdditionalFuncs::rand(const BigInt& N)
{
	srand(time(NULL));
	int l = N.getBILength();
	BigInt res = 0;
	for (int i = 0; i < l + 2; i++)
	{
		int r = std::rand() % 10;
		res = res + r * pow(10, i);
	}
	return res % N;
}

BigInt AdditionalFuncs::gxRho(const BigInt& x, const BigInt& mod)
{
	return (x * x + 1) % mod;
}

BigInt AdditionalFuncs::RhoPollandF(const BigInt& N)
{
	for (BigInt i = 2; i < 100; i = i + 1)
	{
		if (N % i == 0)
		{
			return i;
		}
	}
	BigInt x = 2, y = 2, d = 1;
	while (d == 1)
	{
		x = gxRho(x, N);
		y = gxRho(gxRho(y, N), N);
		d = N.bi_gcd(BigInt::abs(x - y));
	}
	return d;
}

std::pair<BigInt, BigInt> AdditionalFuncs::whatPower(const BigInt& number)
{
	if (number == 2 || number == 3)
	{
		return std::pair<BigInt, BigInt>(number, 1);
	}
	if (MillerRabin(number, number.getBILength()))
	{
		return std::pair<BigInt, BigInt>(number, 1);
	}
	for (BigInt i = 2; i <= number.bi_sqrt(); i = i + 1)
	{
		BigInt counter = 0;
		BigInt t = number;
		while (t % i == 0)
		{
			counter = counter + 1;
			t = t / i;
		}
		if (t == 1)
		{
			return std::pair<BigInt, BigInt>(i, counter);
		}
	}
	return std::pair<BigInt, BigInt>(BigInt(-1), BigInt(0));
}

std::vector<std::pair<BigInt, BigInt>> AdditionalFuncs::Factorize(const BigInt& number)
{
	BigInt nt = number;
	std::vector<std::pair<BigInt, BigInt>> ans;
	//ans.push_back(std::pair<BigInt, BigInt>(1, 0));
	//bool p = false;
	while (nt != 1)
	{
		BigInt d = RhoPollandF(nt);
		std::pair<BigInt, BigInt> temp = whatPower(d);
		nt = nt / d;

		while (nt % temp.first == 0)
		{
			temp.second = temp.second + 1;
			nt = nt / temp.first;
		}
		/*if (!p)
		{
			p = true;
			ans[0] = temp;
		}
		else*/
		ans.push_back(temp);
		if (MillerRabin(nt, nt.getBILength()))
		{
			ans.push_back(std::pair<BigInt, BigInt>(nt, 1));
			return ans;
		}
	}
	return ans;
}

BigInt AdditionalFuncs::EulerFunc(const BigInt& n)
{
	BigInt nt = n;
	BigInt result = n;
	for (BigInt i = 2; i * i <= nt; i = i + 1)
	{
		if (nt % i == 0)
		{
			while (nt % i == 0)
				nt = nt / i;
			result = result - result / i;
		}
	}
	if (nt > 1)
		result = result - result / nt;
	return result;
}

int AdditionalFuncs::MobiusFunc(const BigInt& n)
{
	BigInt nt = n;
	BigInt count = 0;
	BigInt s = n.bi_sqrt();
	for (BigInt i = 2; i <= s; i = i + 1)
	{
		int zer = 0;
		while (nt % i == 0)
		{
			nt = nt / i;
			count = count + 1;
			zer++;
			if (zer >= 2)
				return 0;
		}
	}
	if (nt > 1)
		count = count + 1;
	if (count % 2 == 1)
		return -1;
	return 1;
}

BigInt AdditionalFuncs::ExtendedEuclidianAlgo(const BigInt& a, const BigInt& n)
{
	BigInt x, y;
	auto egcd = BigInt::bi_eegcd(a, n, x, y);
	return (x % n + n) % n;
}

bool AdditionalFuncs::MillerRabin(const BigInt& n, const BigInt& k)
{
	if (n <= 0)
		return false;
	if (n == 1)
		return false;
	if (n == 2 || n == 3 || n == 5)
		return true;
	if (n % 2 == 0)
	{
		//std::cout << "Wrong N" << std::endl;
		return false;
	}
	BigInt d = n - 1;
	BigInt s = 0;
	while (d % 2 == 0)
	{
		s = s + 1;
		d = d / 2;
	}
	for (BigInt i = 0; i < k.bi_sqrt(); i = i + 1)
	{
		bool p = false;
		BigInt a = rand(n - 4) + 2;
		BigInt x = a.powerMod(d, n);
		if (x == 1 || x == n - 1)
			continue;
		for (BigInt j = 0; j < s - 1 ; j = j + 1)
		{
			x = x.powerMod(2, n);
			if (x == 1)
			{
				return false;
			}
			if (x == n - 1)
			{
				p = true;
				break;
			}
		}
		if (p)
			continue;
		return false;
	}
	return true;
}

BigInt AdditionalFuncs::randomPrime()
{
	BigInt temp = rand(BigInt("20000")) + BigInt("10000");
	if (temp % 2 == 0)
	{
		temp = temp - 1;
	}
	while (!MillerRabin(temp, temp.getBILength()))
	{
		temp = temp - 2;
	}
	return temp;
}

BigInt AdditionalFuncs::findRandomNearestPrime(const int bitLength)
{
	BigInt res = BigInt(2).power(bitLength);
	BigInt step = 10;
	int iteration = 0;
	std::cout << res << std::endl;
	while (!MillerRabin(res, res.getBILength()))
	{
		std::cout << "Iteration: " << ++iteration << std::endl;
		res = res - rand(step);
	}
	std::cout << "Prime found: " << res << std::endl;
	return res;
}

BigInt AdditionalFuncs::findPrimitiveRootModule(const BigInt& number)
{
	std::vector<std::pair<BigInt, BigInt>> factors = Factorize(number - 1);
	for (BigInt i = 2; i < number; i = i + 1)
	{
		bool p = false;
		for (std::pair<BigInt, BigInt> j : factors)
			if (i.powerMod((number - 1) / j.first, number) == 1)
				p = true;
		if (p)
			continue;
		return i;
	}
	return 1;
}