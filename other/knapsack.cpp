/**
 * Knapsack
 *
 * Time/Space Complexity: O(Items * Capacity)
 *
 * Decision Problem: NP-complete (can a value of at least V be achieved without exceeding the weight W)
 * Optimization Problem: NP-hard (no polynomial algorithm that can tell if a given solution is optimal)
 */
#include <iostream>
#include <iomanip>
#include <vector>


struct Item
{
	unsigned Value;
	unsigned Weight;

	Item(unsigned v, unsigned w): Value(v), Weight(w) {}
	Item(): Value(0), Weight(0) {}

	friend std::ostream& operator<<(std::ostream& os, const Item& it)
	{
		os << std::setw(2) << it.Weight << "kg - $" << std::setw(2) << it.Value;
		return os;
	}
};


class Knapsack
{
public:
	typedef std::vector<Item> items_t;

public:
	Knapsack(unsigned capacity): m_capacity(capacity), m_value(0) {}

	const Item& add(unsigned val, unsigned w)
	{
		m_items.push_back( Item(val, w) );
		return *m_items.rbegin();
	}

	unsigned value() const { return m_value; }
	const items_t items() const { return m_ks; }

	void calc()
	{
		const unsigned n = m_items.size();

		// Values matrix.
		// 0-th row = 0-th item, 0-th column = 0 weight,
		// therefore they have 0 initial value
		std::vector<unsigned> val((n + 1) * (m_capacity + 1));
		#define K(i,w) val.at(((i) + 1) * (m_capacity + 1) + (w))

		// Compute
		for(unsigned i = 0; i < n; i++)
		{
			const Item& it = m_items[i];

			for(unsigned w = 1; w <= m_capacity; w++)
			{
				unsigned val_prev = K(i - 1, w);
				unsigned val_cur;

				// If enough room...
				if(it.Weight <= w)
				{
					// ...check if the current item would result in a more
					// valuable collection if added to the previous item...
					val_cur = it.Value + K(i - 1, w - it.Weight);
					val_cur = std::max(val_prev, val_cur);
				}
				else
				{
					// ...or just keep the previous item
					val_cur = val_prev;
				}
				K(i, w) = val_cur;
			}
		}

		// Traceback
		m_value = K(n - 1, m_capacity);

		m_ks.clear();
		// Start with the bottom-right (max value/weight) cell
		for(unsigned i = n - 1, w = m_capacity; i < n; i--)
		{
			unsigned val_cur  = K(i    , w);
			unsigned val_prev = K(i - 1, w);
			// If values differ then the item is in the knapsack
			if(val_cur != val_prev)
			{
				m_ks.push_back( m_items[i] );
				w -= m_items[i].Weight;
			}
		}
		#undef K
	}

	friend std::ostream& operator<<(std::ostream& os, const Knapsack& ks)
	{
		for(unsigned i = 0, n = ks.m_ks.size(); i < n; i++)
			std::cout << ks.m_ks[i] << std::endl;
		return os;
	}

private:
	std::vector<Item> m_items;
	unsigned m_capacity;

	// Result
	unsigned m_value;
	items_t m_ks;
};


int main(int, char**)
{
	const unsigned items[][2] =
	{
		// Value, Weight
/*		{ 4, 12},
		{ 2,  1},
		{ 2,  2},
		{10,  4},
		{ 1,  1}
		#define CAPACITY 15
		//*/

		{15, 1},
		{10, 5},
		{ 9, 3},
		{ 5, 4}
		#define CAPACITY 8
		//*/
	};

	Knapsack ks(CAPACITY);

	// Fill
	std::cout << "Initial Items" << std::endl;
	for(unsigned i = 0; i < sizeof(items) / sizeof(*items); i++)
	{
		const Item& it = ks.add(items[i][0], items[i][1]);
		std::cout << it << std::endl;
	}

	// Compute
	ks.calc();

	// Print
	std::cout << "Knapsack" << std::endl;
	std::cout << ks;
	std::cout << "Total $" << ks.value() << std::endl;

	return 0;
}

