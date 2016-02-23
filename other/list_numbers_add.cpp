/**
 * Add numbers as single-linked lists
 */
#include <iostream>


struct Node
{
	Node* next;
	int digit;

	Node(int d, Node* _next): digit(d), next(_next) {}
	~Node() { delete next; }

	unsigned length() const { return 1 + (next ? next->length() : 0); }

	// Convert to integer
	template<typename T>
	T get() const
	{
		T x = 0;
		for(const Node* p = this; p; p = p->next)
			x = x * 10 + p->digit;
		return x;
	}

	friend std::ostream& operator<<(std::ostream& os, const Node& r)
	{
		os << r.digit;
		for(const Node* p = r.next; p; p = p->next)
			os << " -> " << p->digit;
		return os;
	}

	// Add same length
	static Node* add(const Node* a, const Node* b, int* pCarry)
	{
		if(!a && !b)
		{
			*pCarry = 0;
			return nullptr;
		}

		Node* next = add(a->next, b->next, pCarry);
		int y = a->digit + b->digit + *pCarry;
		*pCarry = y / 10;
		return new Node(y % 10, next);
	}

	// Add (inc) carry at a specified position
	static Node* add_carry(const Node* p, int* pCarry, unsigned at, Node* tail)
	{
		if(at == 1)
		{
			int y = p->digit + *pCarry;
			*pCarry = y / 10;
			return new Node(y % 10, tail);
		}

		Node* next = add_carry(p->next, pCarry, at - 1, tail);
		int y = p->digit + *pCarry;
		*pCarry = y / 10;
		return new Node(y % 10, next);
	}
};

// ====================================
template<typename T>
static Node* create_list(T x)
{
	Node* p = nullptr;
	for(T num = x; num; num /= 10)
		p = new Node(num % 10, p);
	return p;
}

Node* add_lists(const Node* a, const Node* b)
{
	unsigned na = a->length();
	unsigned nb = b->length();

	int carry;
	Node* p;

	if(na == nb)
		p = Node::add(a, b, &carry);
	else
	{
		unsigned shift = na - nb;

		const Node *px, *py;
		if(na > nb)
		{
			shift = na - nb;
			px = a;
			py = b;
		}
		else
		{
			shift = nb - na;
			px = b;
			py = a;
		}

		// Add parts with equal lengths first
		const Node* tail = px;
		for(unsigned i = shift; i; --i, tail = tail->next) {}
		p = Node::add(tail, py, &carry);
		// Add a carry to a diff-part
		p = Node::add_carry(px, &carry, shift, p);
	}

	// Add an extra-node if a carry is present
	return carry ? new Node(carry, p) : p;
}

// ====================================
static unsigned urand()
{
	static int dummy = (srand((unsigned)time(nullptr)), 0);

	unsigned x;
	for(unsigned n = 4; n; --n)
		x = (x << 8) + (rand() & 0xFF);
	return x & 0x7FFFFFFF;
}

int main(int, const char**)
{
/*	// Check
	unsigned n;
	for(n = 1024; n; --n)
	{
		unsigned a = urand();
		unsigned b = urand();

		Node* la = create_list(a);
		Node* lb = create_list(b);

		Node* lc = add_lists(la, lb);
		unsigned c = lc->get<unsigned>();
		if(c != a + b)
		{
			std::cout << "ERROR:" << std::endl;
			std::cout << "\t" << a << " + " << b << " = " << (a + b) << std::endl;
			std::cout << "\t(" << *la << ") + (" << *lb << ") = (" << *lc << ") (" << c << ')' << std::endl;
			break;
		}

		delete lc;
		delete lb;
		delete la;
	}
	if(!n)
		std::cout << "OK" << std::endl;
	//*/

	unsigned a, b;
	std::cout << "Input A: "; std::cin >> a;
	std::cout << "Input B: "; std::cin >> b;

	Node* la = create_list(a);
	Node* lb = create_list(b);
	Node* lc = add_lists(la, lb);

	std::cout << "Reference: " << a << " + " << b << " = " << (a + b) << std::endl;
	std::cout << "Lists: (" << *la << ") + (" << *lb << ") = (" << *lc << ") (" << lc->get<unsigned>() << ')' << std::endl;

	delete lc;
	delete lb;
	delete la;
	//*/

	return 0;
}

