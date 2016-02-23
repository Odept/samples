/**
 * Calculate the next number consisting of the same digits as an input
 */
#include <iostream>
#include <vector>


// Count sort is O(N)
static void count_sort_desc(int* nums, size_t sz, unsigned base = 10)
{
	if(sz <= 1)
		return;

	std::vector<unsigned> stat(base);
	for(size_t i = 0; i < sz; ++i)
		++stat[nums[i]];

	int* out = nums;
	for(size_t i = base - 1; i < base; --i)
	{
		for(size_t count = stat[i]; count; --count, ++out)
			*out = i;
	}
}

unsigned inc(unsigned x)
{
	if(x < 10)
		return x;

	// Convert to array
	std::vector<int> digits;
	for(int num = x; num; num /= 10)
		digits.push_back(num % 10);
	size_t l = digits.size();

	// Find the pivot: the first digit[i] < digit[i - 1]
	size_t ipivot;
	for(ipivot = 1; ipivot < l; ++ipivot)
	{
		if(digits[ipivot - 1] > digits[ipivot])
			break;
	}
	// No such digit (already max)?
	if(ipivot == l)
		return x;

	// Find next digit after the pivot (min > pivot)
	size_t inext = ipivot - 1;
	for(int pivot = digits[ipivot]; inext; --inext)
	{
		if(digits[inext - 1] <= pivot)
			break;
	}
	
	// Replace the pivot with the next digit
	std::swap(digits[ipivot], digits[inext]);

	// Sort the rest
	count_sort_desc(&digits[0], ipivot);
	
	// Make an integer
	unsigned y = 0;
	for(unsigned i = 0, factor = 1; i < l; ++i, factor *= 10)
		y += factor * digits[i];
	return y;
}

// ====================================
int main(int, const char**)
{
	unsigned x;// = 135579;

	// Input
	std::cout << "Input a number: ";
	std::cin >> x;
	//std::cout << x << std::endl;

	// Output
	unsigned y = inc(x);
	if(y == x)
		std::cout << "Already the max number" << std::endl;
	else
		std::cout << "The next number with the same digits: " << y << std::endl;

/*	// Check all
	for(unsigned prev = num, next = inc(prev);
		prev != next;
		prev = next, next = inc(next))
	{
		std::cout << next;;
		if(next <= prev)
		{
			std::cout << " - ERROR" << std::endl;
			break;
		}
		std::cout << std::endl;
	}//*/

	return 0;
}

