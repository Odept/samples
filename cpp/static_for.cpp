/**
 * static_for
 *
 * Based on templates (works for <255 iterationsi). Uses a passed callable
 * object (i.e. function pointer or lambda) for a per-iteration execution.
 */
#include <iostream>


template<int I, int N, typename FUNC>
struct static_for_impl
{
	static_for_impl(FUNC pfn)
	{
		pfn(I);
		static_for_impl<I + 1, N, FUNC> o(pfn);
	}
};
// Template specialization for the terminating iteration
template<int N, typename FUNC>
struct static_for_impl<N, N, FUNC>
{
	static_for_impl(FUNC) {}
};

// Interface function
template<int X, int TO, typename FUNC>
void static_for(FUNC pfn)
{
	static_for_impl<X, TO, FUNC> o(pfn);
}

// ====================================
void print(int i)
{
	std::cout << "function: iteration" << i << std::endl;
}

int main(int, const char**)
{
    static_for<0, 13>(print);
    static_for<0, 8>(
		[]
		(int i)
		{
			std::cout << "lambda: iteration" << i << std::endl;
		}
	);

	return 0;
}

