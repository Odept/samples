#include <iostream>
#include <vector>
#include <cstdlib>

#include "sort.h"
#include "search.h"


static unsigned urand()
{
	static int dummy = (srand(time(NULL)), 0);
	return (((rand() & 0x7FFF) << 17) | ((rand() & 0x7FFF) << 2) << (rand() & 0x3));
	//return (rand() & 0x3);
	//return arc4random();
}

static void irand(int* a, unsigned n)
{
	for(unsigned i = 0; i < n; i++)
		a[i] = (int)urand();
}

// ====================================
static void check_sort(unsigned n)
{
	std::vector<int> a(n);
	irand(&a[0], n);

	// Print unsorted
	/*std::cout << "Unsorted:";
	for(unsigned i = 0; i < n; i++)
		std::cout << ' ' << a[i];
	std::cout << std::endl;
	//*/
	
	// Sort
	clock_t c = clock();

	//sort_sel(&a[0], n);
	//sort_ins(&a[0], n);
	//sort_bub(&a[0], n);
	//sort_shell(&a[0], n);
	//sort_quick(&a[0], n);
	sort_merge(&a[0], n);

	c = clock() - c;

	// Print sorted
	/*std::cout << "Sorted:";
	for(unsigned i = 0; i < n; i++)
		std::cout << ' ' << a[i];
	std::cout << std::endl;
	//*/

	// Check
	std::cout << "Sort: " << (is_sorted(&a[0], n) ? "OK" : "FAILED!!!") <<
				 " (" << c / (float)CLOCKS_PER_SEC << " sec (" << (c / n) << "))" << std::endl;
}

// ====================================
static void check_search(unsigned n)
{
	std::vector<int> a(n);
	irand(&a[0], n);

	// ========
	//sort_quick(&a[0], n);
	//#define FIND(k, v, n) find_binary(k, &v[0], n)
	// ========
	BST<int,int> s(&a[0], n);
	#define FIND(k, v, n) s.find(k)
	// ========

	int k;
	int* pk;

	// First hit check
	k = a[0];
	pk = FIND(k, a, n);
	std::cout << "Search (hit first): " << ((pk && *pk == k) ? "OK" : "FAILED!!!") << std::endl;

	// Last hit check
	k = a[n - 1];
	pk = FIND(k, a, n);
	std::cout << "Search (hit last): " << ((pk && *pk == k) ? "OK" : "FAILED!!!") << std::endl;
	
	// Random hit check
	k = a[urand() % n];
	pk = FIND(k, a, n);
	std::cout << "Search (hit random): " << ((pk && *pk == k) ? "OK" : "FAILED!!!") << std::endl;

	// Miss check
	unsigned ki;

	for(ki = urand() % n; 
		!ki || a[ki - 1] == a[ki] - 1;
		ki = urand() % n)
	{}
	k = a[ki] - 1;

	pk = FIND(k, a, n);
	std::cout << "Search (miss): " << (pk ? "FAILED!!!" : "OK") << std::endl;
}

// ====================================
int main(int, char**)
{
	const unsigned N = 1024 * 1024 - 1;

	check_sort(N);
	check_search(N);

	return 0;
}

