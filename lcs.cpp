/**
 * Longest Common Subsequence
 *
 * Time Complexity: O(Len0 * Len1)
 * Space Complexity:
 *	- length only case		O(Len0 * Len1)
 *	- traceback all case	O(Len0 * Len1 * ? * LenSeq)
 *
 * NP-complete
 */
#include <iostream>
#include <vector>


class LCS
{
private:
	typedef std::vector<std::string> strings_t;

	struct Cell
	{
		unsigned Length;
		strings_t Strings;

		Cell(): Length(0) {}

		bool operator>(const Cell& f_cell) const { return (Length > f_cell.Length); }

		// Append a character to all the strings in the cell
		void operator+=(char f_c)
		{
			if(unsigned n = Strings.size())
			{
				for(unsigned i = 0; i < n; i++)
					Strings[i].append(1, f_c);
			}
			else
				Strings.push_back( std::string(&f_c, 1) );
			Length++;
		}

		// Check if the specified string is present in the list
		bool operator!=(const std::string& f_str) const
		{
			for(unsigned i = 0, n = Strings.size(); i < n; i++)
			{
				if(Strings[i] == f_str)
					return false;
			}
			return true;
		}
		// Merge two cells omitting duplicates
		friend Cell operator+(const Cell& a, const Cell& b)
		{
			Cell x = a;
			for(unsigned i = 0, n = b.Strings.size(); i < n; i++)
			{
				const std::string& str = b.Strings[i];
				// Not optimal: it's enough to compare with only N initial strings
				if(x != str)
					x.Strings.push_back(str);
			}
			return x;
		}
	};

public:
	LCS(const std::string& f_strA, const std::string& f_strB)
	{
		unsigned n = f_strA.length();
		unsigned m = f_strB.length();
		if(!n || !m)
			return;

		std::vector<Cell> mat((n + 1) * (m + 1));
		#define CELL(y, x) mat[(y + 1) * (m + 1) + (x + 1)]

		for(unsigned i = 0; i < n; i++)
		{
			char cA = f_strA[i];

			for(unsigned j = 0; j < m; j++)
			{
				Cell& cell = CELL(i, j);

				if(f_strB[j] == cA)
				{
					cell = CELL(i - 1, j - 1);
					cell += cA;
				}
				else
				{
					const Cell& up   = CELL(i - 1, j    );
					const Cell& left = CELL(i    , j - 1);
					if(left > up)
						cell = left;
					else if(up > left)
						cell = up;
					else
						cell = left + up;
				}
			}
		}

		m_strings = CELL(n - 1, m - 1).Strings;
		#undef M
	}

	unsigned count() const { return m_strings.size(); }

	friend std::ostream& operator<<(std::ostream& os, const LCS& f_lcs)
	{
		if(unsigned n = f_lcs.m_strings.size())
		{
			unsigned n1 = n - 1;
			for(unsigned i = 0; i < n1; i++)
				os << f_lcs.m_strings[i] << ' ';
			os << f_lcs.m_strings[n1];
		}
		return os;
	}

private:
	strings_t m_strings;
};

// ====================================
int main(int, char**)
{
	std::string x, y;

	// Input
	//x = "gac"; y = "agcat";
	//x = "xmjyauz"; y = "mzjawxu";
	std::cout << "First  string: "; std::cin >> x;
	std::cout << "Second string: "; std::cin >> y;

	LCS lcs(x, y);
	if(lcs.count())
		std::cout << "Longest Common Subsequnce(s) of \"" << x << "\" and \"" << y << "\": " << lcs << std::endl;
	else
		std::cout << '\"' << x << "\" and \"" << y << "\" don't have any common subsequnces" << std::endl;

	return 0;
}

