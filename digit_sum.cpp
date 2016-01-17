/**
 * Given a non-negative integer num, repeatedly add all its digits until the result has only one digit.
 *
 * Naive: Otime(logR + loglogR + logloglogR + ...)
 * Digital Root Algorithm: Otime(logR*) ~ O(1)
 */
#include <iostream>
#include <sstream>
#include <cstdlib>

#include <time.h>

int main(int, char**)
{
	srand(time(NULL));

	// Gen number
	unsigned N = 32 + rand() % 64;
	std::ostringstream oss;
	for(unsigned n = N; n; n--)
		oss << (rand() % 9 + '0');
	std::string num = oss.str();

	std::cout << "Number: " << num << std::endl;

	// Naive
	{
		// The first step: "reduce" literally any number to an unsigned int value
		unsigned x = 0;
		for(unsigned i = 0, n = num.length(); i < n; i++)
			x += num[i] - '0';

		// Loop summing up digits while the result >= 10
		while(x > 9)
		{
			unsigned y = 0;
			while(x)
			{
				y += x % 10;
				x /= 10;
			}
			x = y;
		}

		std::cout << "Naive: " << x << std::endl;
	}

	/**
	 * Digital Root
	 * Consider 53462. Cut out n numbers that sum up to 9: x3x62 -> xyxy2 -> result is 2.
	 * What about 53472? 53472 -> x3x72 -> ?. Looking a bit closer one can find
	 * a more general rule: having a 9 in the number won’t have any impact in the digital sum.
	 *
	 * The result is digit sum mod 9 BUT that is "equivalent" (???) to just mod 9... almost!
	 * Consider 36: 36 mod 9 = 0, however 9 is expected.
	 * Therefore calc x-1 mod 9 (the number of digits in x-1) and add 1 back:
	 *	x mod BASE E [0; BASE),
	 *	so max(x mod BASE) = BASE-1,
	 *	therefore max(x mod 9)+1 = BASE.
	 */
	{
		// The first step is the same as in the naive algorithm: reduce to an unsigned int
		unsigned x = 0;
		for(unsigned i = 0, n = num.length(); i < n; i++)
			x += num[i] - '0';

		// Do the trick
		x = (1 + (x - 1) % 9);

		std::cout << "Digital Root: " << x << std::endl;
	}

	return 0;
}

