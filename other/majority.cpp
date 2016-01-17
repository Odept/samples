/**
 * Given an array of size n, find the MAJORITY element.
 * The majority element is the element that appears more than n/2 times.
 *
 * std::map: Otime(NlogN), Ospace(N)
 * Boyer–Moore voting algorithm: Otime(n), Ospace(1)
 */
#include <iostream>
#include <cstdlib>

#include <time.h>

#include <vector>
#include <map>


int main(int, char**)
{
	srand(time(NULL));

	unsigned N = 256 + (rand() & 1);
	std::vector<int> a(N);

	// Fill
	int m = rand() % 4096 - 2048;
	int n = N / 2 + (rand() % 4 - 1);

	unsigned i;
	for(i = 0; i < n; ++i)
		a[i] = m;
	for(; i < N; ++i)
		a[i] = (rand() % 4096) - 2048;
	std::random_shuffle(a.begin(), a.end());

	std::cout << "Expected: ";
	if(n > N / 2)
		std::cout << m;
	else
		std::cout << "no candidate";
	std::cout << " (" << n << " of " << N << ')' << std::endl;

	// std::map
	std::cout << "std::map: ";
	{
		typedef std::map<int, int> stat_t;
		stat_t stat;

		// Fill the map
		for(unsigned i = 0; i < N; i++)
			++stat[ a[i] ];

		// Check & Print
		stat_t::iterator it, end;
		for(it = stat.begin(), end = stat.end(); it != end; it++)
		{
			if(it->second > N / 2)
			{
				std::cout << it->first << " - " << it->second << " occurrences";
				break;
			}
		}
		if(it == end)
			std::cout << "not found";
		std::cout << std::endl;
	}
	// Boyer–Moore
	std::cout << "Boyer–Moore: ";
	{
		unsigned c = 0;
		int val;

		// Find possible candidate
		for(unsigned i = 0; i < N; i++)
		{
			if(c == 0)
			{
				val = a[i];
				c = 1;
			}
			else if(a[i] == val)
				++c;
			else
				--c;
		}

		// Check & Print
		c = 0;
		for(unsigned i = 0; i < N; i++)
		{
			if(a[i] == val)
				++c;
		}
		if(c > N / 2)
			std::cout << val << " - " << c << " occurrences";
		else
			std::cout << "not found";
		std::cout << std::endl;
	}

	return 0;
}

