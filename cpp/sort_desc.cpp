#include <iostream>

#include <vector>


template<typename T>
class TypeChecker;
#define CHECK_TYPE(expr) TypeChecker<decltype(expr)> check;


// std::greater comparator
template<typename T>
void sort_desc_stdcmp(T& v)
{
	std::sort(v.begin(), v.end(), std::greater<typename T::value_type>());
}

// Class comparator which uses "operator()"
template<typename T>
void sort_desc_classcmp(T& v)
{
	// Local type is not a part of C++03 standard
	struct cmp_greater
	{
		bool operator()(const decltype(v[0])& a, const decltype(v[0])& b) const { return b < a; }
	};
	std::sort(v.begin(), v.end(), cmp_greater());
}

// Reverse iterator
template<typename T>
void sort_desc_rit(T& v)
{
	std::sort(v.rbegin(), v.rend());
}

// Usual sort + reverse
template<typename T>
void sort_desc_rev(T& v)
{
	std::sort(v.begin(), v.end());
	std::reverse(v.begin(), v.end());
}

// Lambda expression comparator
template<typename T>
void sort_desc_lambda(T& v)
{
	std::sort(v.begin(), v.end(),
		[]
		(const decltype(v[0])& a, const decltype(v[0])& b)
		{
			return b < a;
		}
	);
}

// ====================================
template<typename T>
static bool isDescending(const T& v)
{
	size_t i, n = v.size();
	for(i = 1; i < n; i++)
	{
		if(v[i - 1] < v[i])
			break;
	}
	return (i == n);
}

int main(int, const char**)
{
	static const unsigned N = 512;
	srand(time(nullptr));

	// Init data
	std::vector<int> v(N);
	for(unsigned i = 0; i < N; ++i)
		v[i] = rand() % 8192 - 4096;

	// Check
	for(unsigned i = 0; i < 5; ++i)
	{
		auto x = v;
		switch(i)
		{
			case 0:
				sort_desc_stdcmp(x);
				std::cout << "std::greater     : " << (isDescending(x) ? "OK" : "ERROR") << std::endl;
				break;
			case 1:
				sort_desc_classcmp(x);
				std::cout << "class::operator(): " << (isDescending(x) ? "OK" : "ERROR") << std::endl;
				break;
			case 2:
				sort_desc_rit(x);
				std::cout << "reverse_iterator : " << (isDescending(x) ? "OK" : "ERROR") << std::endl;
				break;
			case 3:
				sort_desc_rev(x);
				std::cout << "std::reverse     : " << (isDescending(x) ? "OK" : "ERROR") << std::endl;
				break;
			case 4:
				sort_desc_lambda(x);
				std::cout << "lambda           : " << (isDescending(x) ? "OK" : "ERROR") << std::endl;
				break;
		}
	}

	return 0;
}

