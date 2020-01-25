#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>
#include <fstream>
#include <chrono>
#include <time.h>
#include <random>

struct dot
{
	double x;
	double y;
	int number;

	double operator[](int i) const
	{
		return i ? y : x;
	}

	double& operator[](int i)
	{
		return i ? y : x;
	}

	bool operator == (const dot &o)
	{
		return x == o.x && y == o.y;
	}

	bool operator < (const dot &o)
	{
		return x < o.x || (x == o.x && y < o.y);
	}

};

std::vector<dot> generateDots( std::size_t size)
{
	std::vector<dot> res;
	for (int i = 0; i < size; ++i)
	{
		double x, y;
		x = ((double)rand() / (RAND_MAX)) * 100;
		y = ((double)rand() / (RAND_MAX)) * 100;
		res.push_back(dot{ x, y, i });
	}
	return res;
}

double rotateZ(const dot &a, const dot &b, const dot &c)
{
	return (b[0] - a[0]) * (c[1] - b[1]) - (b[1] - a[1]) * (c[0] - b[0]);
}

std::vector<dot> grahamScan(std::vector<dot>& dots)
{
	int size = dots.size();
	if (size < 3)
		return dots;

	std::vector<int> inds(size);
	for (int i = 0; i < size; ++i)
		inds[i] = i;

	int min_ind = 0;
	for (int i = 0; i < size; ++i)
		if (dots[i][0] < dots[min_ind][0])
			min_ind = i;

	int tInd = inds[0];
	inds[0] = inds[min_ind];
	inds[min_ind] = tInd;

	for (int i = 2; i < size; ++i)
	{
		int j = i;
		while (j > 1 && rotateZ(dots[inds[0]], dots[inds[j - 1]], dots[inds[j]]) < 0)
		{
			int tInd = inds[j - 1];
			inds[j - 1] = inds[j];
			inds[j] = tInd;
			j--;
		}
	}

	std::vector<int> tres;
	tres.push_back(inds[0]);
	tres.push_back(inds[1]);
	std::vector<dot> res;
	res.push_back(dots[inds[0]]);
	res.push_back(dots[inds[1]]);

	for (int i = 2; i < size; ++i)
	{
		while (rotateZ(dots[tres[tres.size() - 2]], dots[tres[tres.size() - 1]], dots[inds[i]]) < 0)
		{
			tres.pop_back();
			res.pop_back();
		}
		tres.push_back(inds[i]);
		res.push_back(dots[inds[i]]);
	}
	return res;
}


int main()
{
	srand(1);
	std::ofstream out("outputs.txt");
	if (out.is_open())
	{
		for (std::size_t i = 1; i <= 5000; ++i)
		{
			const std::size_t number = i;
			auto d = generateDots(number);
			auto start = std::chrono::system_clock::now();
			grahamScan(d);
			auto finish = std::chrono::system_clock::now();
			std::chrono::duration<double> diff = finish - start;
			out << number << ' ' << diff.count() << '\n';
		}
	}
	return 0;
}