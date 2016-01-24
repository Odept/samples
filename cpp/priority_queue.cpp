/**
 * Priority Queue
 *
 * Complexity:
 *	push/pop - O(logN)
 */
#include <iostream>
#include <vector>


template<class T>
class PriorityQueue
{
public:
	PriorityQueue(): m_data(1) {}
	virtual ~PriorityQueue() {}

	size_t size() const { return (m_data.size() - 1); }
	bool empty() const { return (m_data.size() == 1); }
	const T& top() const { return m_data.at(size()); }

	void push(const T& f_o)
	{
		size_t iCur = m_data.size();
		m_data.push_back(f_o);
		for(size_t iParent = iCur / 2; iParent; iCur = iParent, iParent = iCur / 2)
		{
			if( !cmp_xchg_less(m_data[iParent], m_data[iCur]) )
				break;
		}
	}
	void pop()
	{
		if(size_t n = size())
		{
			m_data[1] = m_data[n];
			m_data.resize(n);
			if(n > 1)
				fixDown();
		}
	}

	const T& operator[](size_t i) const { return m_data.at(i + 1); }
	      T& operator[](size_t i)       { return m_data.at(i + 1); }

private:
	bool cmp_xchg_less(T& a, T& b)
	{
		if(a < b)
		{
			T c = a;
			a = b;
			b = c;
			return true;
		}
		else
			return false;
	}

	void fixDown(size_t i = 1)
	{
		size_t n = m_data.size();
		size_t iChild = i * 2;
		if(iChild < n)
		{
			if(i * 2 + 1 < n && m_data[iChild] < m_data[iChild + 1])
				++iChild;
			if( cmp_xchg_less(m_data[i], m_data[iChild]) )
				fixDown(iChild);
		}
	}

private:
	std::vector<T> m_data;
};

// ====================================
int main(int, char**)
{
	srand(time(NULL));

	PriorityQueue<int> pq;

	// Fill the queue
	static const unsigned N = 4096;
	for(unsigned n = N; n; n--)
	{
		int x = static_cast<int>(rand() % 1024) - 512;
		//std::cout << x << std::endl;
		pq.push(x);
	}

	// Check
	for(int mx = std::numeric_limits<int>::max(); !pq.empty(); pq.pop())
	{
		if(pq.top() > mx)
			break;
	}
	std::cout << (pq.empty() ? "OK" : "ERROR") << std::endl;

	return 0;
}

