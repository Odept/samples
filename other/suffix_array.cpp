/**
 * Suffix Array (+ Longest Common Prefixes)
 *
 * A Suffix Array contains integers which represent the starting indices
 * of all the suffixes sorted in a lexicographic order.
 * An LCP array contains the lengths of prefixes that are common between
 * the two consecutive suffixes in a sorted suffix array.
 *
 * Basic approach:
 *		Get all the suffixes and sort lexicographically
 *		Otime(N^2*logN) = Osort(NlogN) * Ostrcmp(strlen)
 * Advanced approach:
 *		Otime(Nlog^2(N)) = Osort(NlogN) * Oloop(logN)
 */
#include <iostream>
#include <algorithm>
#include <vector>


class SuffixArray
{
private:
	/**
	 * Cumulative comparison info.
	 * Rank - current char rank
	 * RankNext - cumulative rank for the next comparison char
	 */
	struct CmpInfo
	{
		unsigned Rank;
		unsigned RankNext;
		unsigned Index;

		bool operator< (const CmpInfo& o) const { return ((Rank == o.Rank && RankNext < o.RankNext) || Rank < o.Rank); }
		bool operator!=(const CmpInfo& o) const { return (Rank != o.Rank || RankNext != o.RankNext); }
	};

public:
	SuffixArray(const std::string& str):
		m_str(str),
		m_indices(str.length()),
		m_lcp(str.length())
	{
		const unsigned len = str.length();

		/**
		 * Init lexicographic ranks.
		 * Rank 0 is reserved for the highest rank.
		 * Lower value - higher rank/priority.
		 */
		static const unsigned maxRank = 0;

		std::vector<unsigned> ranks(len);
		for(unsigned i = 0; i < len; i++)
			ranks[i] = (maxRank + 1) + str[i] - 'a';

		// Compare char pairs separated by the offset.
		std::vector<CmpInfo> info(len);

		for(unsigned offset = 1; offset < len; offset *= 2)
		{
			// Update comparison info with the previous iteration results
			for(unsigned i = 0; i < len; i++)
			{
				info[i].Rank = ranks[i];
				if(i + offset < len)
					info[i].RankNext = ranks[i + offset];
				else
					info[i].RankNext = maxRank;
				info[i].Index = i;
			}

			// Sort suffixes based on lexicographic ranks
			std::sort(info.begin(), info.end());

			/**
			 * Re-rank based on the sorted order.
			 * The new rank increments each time the consecutive suffxes
			 * are lexicographically different *for the current iteration*
			 */
			unsigned r = maxRank + 1;;

			ranks[info[0].Index] = r;
			for(unsigned i = 1; i < len; ++i)
			{
				if(info[i - 1] != info[i])
					++r;
				ranks[info[i].Index] = r;
			}
		}

		// Save the result
		const char* cstr = m_str.c_str();

		m_lcp[0] = 0;
		m_indices[0] = info[0].Index;
		for(unsigned i = 1; i < len; ++i)
		{
			m_indices[i] = info[i].Index;

			// Compute LCP length
			const char* suf_cur  = cstr + m_indices[i];
			const char* suf_prev = cstr + m_indices[i - 1];

			unsigned lcp = 0;
			for(unsigned n = len - std::max(m_indices[i], m_indices[i - 1]); lcp < n; lcp++)
			{
				if(suf_cur[lcp] != suf_prev[lcp])
					break;
			}
			m_lcp[i] = lcp;
		}
	}

	unsigned index(unsigned i) const { return m_indices.at(i); }
	const char* operator[](unsigned i) const { return (m_str.c_str() + index(i)); }
	unsigned lcp(unsigned i) { return m_lcp.at(i); }

private:
	const std::string m_str;
	std::vector<unsigned> m_indices;

	// Longest Common (with the previous suffix) Prefix length
	std::vector<unsigned> m_lcp;
};

// ====================================
int main(int, char**)
{
	std::string str = "abcxeabcye";
	//std::string str = "judaioobpoiteiszvzlscmpmpqqwuvtdqzdapudfimaowsnttalwndievaapwusmtyoksrpcfpqbkgvfiibvlkbjkcy";
	std::cout << "Input: " << str << std::endl;

	std::cout << "Suffix Array (+ LCPs):" << std::endl;
	SuffixArray sa(str);
	for(unsigned i = 0; i < str.length(); i++)
		std::cout << '\t' << sa.lcp(i) << " | " << sa[i] << std::endl;
}

