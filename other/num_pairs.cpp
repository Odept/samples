/**
 * Find all the pairs in an array that sum to X
 *
 * The first approach is to sort the array, and then
 * iterate with two pointers from both ends until they meet:
 *		if a current sum is > X
 *			then end--,
 *		else if > X
 *			then begin++,
 *		else
 *			begin++, end--.
 * Complexity: Otime(NlogN) = Osort(NlogN) + Oit(N); Ospace(1)
 *
 * The second approch is to iterate over an array:
 * 1. If the element which conjugate to the current (Sum - x)
 *	  has already been seen then a pair is found;
 * 2. Mark (add to a hash table) the current element (so it can be used
 *	  as a conjugate of another element on some further iteration).
 * Note that if all the numbers are limited by some range
 * then a bool vector can be used insted of a hash table.
 * Complexity: Otime(n); Ospace(N)
 */
#include <iostream>
#include <cstdlib>

#include <vector>
#include <set>


template<class T>
class PRNArray
{
public:
	PRNArray(unsigned n, const T& val_min, const T& val_max):
		m_array(n)
	{
		T mod = val_max - val_min;
		std::set<T> hash;

		for(unsigned i = 0; i < n; i++)
		{
			int x;
			do
				x = rand() % mod + val_min;
			while(!hash.insert(x).second);

			m_array[i] = x;
		}
	}

	const T& operator[](unsigned i) { return m_array[i]; }

	friend std::ostream& operator<<(std::ostream& os, const PRNArray& o)
	{
		if(!o.m_array.empty())
		{
			for(unsigned i = 0, n = o.m_array.size() - 1; i < n; i++)
				os << o.m_array[i] << ' ';
			os << *o.m_array.rbegin();
		}
		return os;
	}

private:
	std::vector<T> m_array;
};

// ====================================
int main(int, char**)
{
	srand(time(NULL));

	static const unsigned N = 48;
    static const unsigned Sum = 7;

	// Generate distinct numbers
	PRNArray<int> nums(N, -(int)N, N);
	std::cout << "Array: " << nums << std::endl;

    // Use a hash table to match paired elements
	typedef std::set<int> hash_t;
	hash_t hash;
	hash_t::const_iterator end = hash.end();

	bool bFound = false;
    std::cout << "Pairs that sum to " << Sum << ":";
    for(unsigned i = 0; i < N; i++)
    {
        int x = nums[i];
		/**
		 * Note:
		 *	if all the values are >= 0, the loop
		 *  might be sped up by skipping values > Sum
		 */
        int y = Sum - x;

        /**
		 * Print a pair only when the second element
		 * of the pair has already been observed
		 */
		hash_t::iterator it = hash.find(y);
        if(it != hash.end())
		{
            std::cout << "  (" << y << "; " << x;
			if(y + x != Sum)
				std::cout << " ERROR";
			std::cout << ')';
			bFound = true;
		}
		hash.insert(x);
    }
	if(!bFound)
		std::cout << " none";
	std::cout << std::endl;

	return 0;
}

