/**
 * Select a minimum number of ranges that provide maximum cover
 *
 * Complexity: Otime(N)
 */
#include <iostream>

#include <vector>

/**
 * Note that both of the following sorting criteria are not optimal.
 * Consider the following intervals:
 * ______               1
 * _______________      2
 *                  ___ 3
 * 2 & 3 are selected when sorted by max End that is optimal,
 * and ALL of them are selected otherwise, BUT:
 * ______               1
 * _______________      2
 *      _______________ 3
 *                  ___ 4
 * 2 & 3 are selected when sorted by max End, that is NOT optimal -
 * 1 & 3 would be better (and that is done when sorting without
 * considering End-s).
 *
 * Anyway, take End-s into an account while sorting, because it's
 * at least a deterministic solution.
 */
struct Range
{
	unsigned Start;
	unsigned End;
	//static bool less_start(const Range& a, const Range& b) { return (a.Start < b.Start); }
	static bool less_start(const Range& a, const Range& b) { return (a.Start < b.Start) || (a.Start == b.Start && a.End >/*!!!*/ b.End); }
};
typedef std::vector<Range> ranges_t;


ranges_t max_cover(ranges_t in)
{
	ranges_t out;

	unsigned n = in.size();
	if(!n)
		return out;

	// Sort by starting position
	sort(in.begin(), in.end(), Range::less_start);

	// Save the first element
	out.push_back(in[0]);
	// Use the first element as the current
	unsigned end = in[0].End;
	// Index of the max-end range which intersects the current range
	// (n = none)
	unsigned j = n;

	for(unsigned i = 1; i < n;)
	{
		const Range& x = in[i];
		// Skip ranges inside the current
		if(x.End <= end)
		{
			++i;
			continue;
		}

		// If the new current range
		if(x.Start > end)
		{
			// Save max-end range within the previous current range
			if(j != n)
			{
				out.push_back(in[j]);
				end = in[j].End;
				j = n;
				continue;
			}
			// Save new current
			out.push_back(in[i]);
			end = x.End;
		}
		// Update max-end range within the current range
		else if(j == n || x.End >= in[j].End)
			j = i;
		++i;
	}
	if(j != n)
		out.push_back(in[j]);

	return out;
}

// ====================================
static std::ostream& operator<<(std::ostream& os, const ranges_t ranges)
{
	unsigned m = 0;
	for(unsigned i = 0, n = ranges.size(), j; i < n; ++i)
	{
		const Range& x = ranges[i];
		for(j = 0; j < x.Start; ++j)
			os << ' ';
		for(; j < x.End; ++j)
			os << '_';
		std::cout << std::endl;
		// Get max end
		if(x.End > m)
			m = x.End;
	}
	for(unsigned i = 0; i < m; ++i)
		os << '=';
	os << std::endl;

	return os;
}

int main(int, char**)
{
	const Range r[] =
	{
		{ 0,  6},
		{ 0, 15},
		{ 5, 20},
		{17, 20}
	//*/
	/*	{1, 3},
		{1, 7},
		{5, 9}
	//*/
	/*	{22, 28},
		{12, 16},
		{ 8, 18},
		{20, 24},
		{ 2, 14},
		{14, 26},
		{ 6, 10},
		{30, 32},
		{ 2, 15},
	//*/
	};
	ranges_t ranges;
	for(unsigned i = 0; i < sizeof(r) / sizeof(*r); ++i)
		ranges.push_back(r[i]);

	// Print input
	std::cout << "Ranges:" << std::endl << ranges;

	// Solve
	ranges_t rr = max_cover(ranges);

	// Print output
	std::cout << "Maximum cover:" << std::endl << rr;

	return 0;
}

