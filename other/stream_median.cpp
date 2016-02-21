/**
 * Median of a stream
 *
 * Direct solution:
 *	insert - O(1)
 *	get    - Osort(NlogN)
 * Dual priority queue:
 *	insert - O(logN)
 *	get    - O(1)
 */
#include <iostream>

#include <vector>
#include <queue>


template<typename T>
using stream_t = std::vector<T>;

template<typename T>
static float median(const stream_t<T>& f_stream)
{
	stream_t<T> s(f_stream);
	std::sort(s.begin(), s.end());

	size_t sz = s.size();
	if(sz & 1)
		return s[sz / 2];
	else
		return (s[sz / 2 - 1] + s[sz / 2]) / 2.0f;
}

// ====================================
template<typename T>
class StreamMedian
{
public:
	void push(const T& x)
	{
		// Select the part to insert into
		if(m_pq_min.empty() || x < m_pq_min.top())
			m_pq_min.push(x);
		else
			m_pq_max.push(x);

		// Balance
		size_t sz_min = m_pq_min.size();
		size_t sz_max = m_pq_max.size();
		if(sz_min < sz_max)
		{
			T a = m_pq_max.top();
			m_pq_max.pop();
			m_pq_min.push(a);
		}
		else if(sz_min > sz_max + 1)
		{
			T a = m_pq_min.top();
			m_pq_min.pop();
			m_pq_max.push(a);
		}
	}

	float get() const
	{
		if(m_pq_min.size() == m_pq_max.size())
			return (m_pq_min.top() + m_pq_max.top()) / 2.0f;
		else
			return m_pq_min.top();
	}

private:
	std::priority_queue<T> m_pq_min;
	std::priority_queue<T, std::vector<T>, std::greater<T>> m_pq_max;
};


int main(int, char**)
{
	constexpr unsigned N = 256;
	srand(time(nullptr));
	const int offset = rand() % 8192 - 4096;

	// Median-stream and a reference stream
	StreamMedian<int> sm;
	stream_t<int> s;

	// Proceed
	unsigned i;
	for(i = 0; i < N; ++i)
	{
		int x = rand() % 4096 + offset;

		sm.push(x);
		s.push_back(x);

		// Just verify
		if(median(s) != sm.get())
		{
			std::cout << "ERROR @ " << i << std::endl;
			break;
		}
	}
	if(i == N)
		std::cout << "stream median = " << sm.get() << std::endl;

	return 0;
}

