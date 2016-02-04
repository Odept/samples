/**
 * Reverse a single-linked list
 */
#include <iostream>


template<typename T>
class List
{
public:
	struct Node
	{
		T data;
		Node* next;
		Node(const T& x, Node* _next): data(x), next(_next) {}
	};

	List(): m_head(nullptr) {}
	~List()
	{
		for(Node* p = m_head; p;)
		{
			Node* cur = p;
			p = p->next;
			delete cur;
		}
	}

	//T* head() { return m_head; }
	void push_head(const T& x) { m_head = new Node(x, m_head); }

	void reverse()
	{
		Node* rhead = nullptr;
		for(Node* p = m_head; p;)
		{
			Node* t = p->next;
			p->next = rhead;
			rhead = p;
			p = t;
		}
		m_head = rhead;
	}

	friend std::ostream& operator<<(std::ostream& os, const List& l)
	{
		Node* h = l.m_head;
		if(h)
			os << h->data;
		for(h = h->next; h; h = h->next)
			os << " -> " << h->data;
		return os;
	}

private:
	Node* m_head;
};

// ====================================
int main(int, char**)
{
	static const unsigned N = 8;
	srand(time(nullptr));

	List<int> list;

	// Fill list with random data
	for(unsigned i = 0; i < N; i++)
		list.push_head(rand() % 1024 - 512);
	
	// Print initial
	std::cout << "Initial : " << list << std::endl;

	// Do
	list.reverse();

	// Print reversed
	std::cout << "Reversed: " << list << std::endl;

	return 0;
}

