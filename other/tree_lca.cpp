/**
 * Find the LCA (Lowest Common Ancestor) of given nodes in a binary tree.
 */
#include <iostream>
#include <iomanip>
#include <cstdlib>

#include <stack>

// Node & Tree functions
struct Node
{
	Node *l, *r;
	Node *p;
	int N;
	unsigned L;
	Node(Node* _p, int x, unsigned lv): p(_p), N(x), L(lv), l(nullptr), r(nullptr) {}
};

static void insert(Node*& p, Node* parent, int x, unsigned level = 0)
{
	if(p)
	{
		if(rand() & 1)
			insert(p->l, p, x, level + 1);
		else
			insert(p->r, p, x, level + 1);
	}
	else
		p = new Node(parent, x, level);
}

Node* find(Node* p, int x)
{
	if(!p)
		return nullptr;

	if(p->N == x)
		return p;
	if(Node* l = find(p->l, x))
		return l;
	if(Node* r = find(p->r, x))
		return r;
	return nullptr;
}

// ====================================
class LCA
{
public:
	LCA(Node* root, int x, int y, bool use_parent):
		m_p(nullptr)
	{
		if(!root)
			return;
		if(use_parent)
			m_p = traverse(find(root, x), find(root, y));
		else
		{
			Node* a = find(root, x);
			Node* b = find(root, y);
			if(!a || !b)
				return;
			m_p = traverse(root, a, b);
		}
	}

	const Node* get() const { return m_p; }

private:
	// Solution for nodes w/o the parent pointer
	Node* traverse(Node* cur, const Node* x, const Node* y)
	{
		if(!cur)
			return nullptr;

		if(cur == x || cur == y)
			return cur;

		Node* pR = traverse(cur->l, x, y);
		Node* pL = traverse(cur->r, x, y);

		if(pR && pL)
			return cur;
		return pR ? pR : pL;
	}

	// Solution for nodes w/ the parent pointer
	Node* traverse(Node* x, Node* y)
	{
		typedef std::stack<Node*> backtrace_t;

		if(!x || !y)
			return nullptr;

		// The first backtrace
		backtrace_t bt_x;
		for(auto p = x; p; p = p->p)
			bt_x.push(p);

		// The second backtrace
		backtrace_t bt_y;
		for(auto p = y; p; p = p->p)
			bt_y.push(p);

		// Loop while backtraces are equal
		Node* z = nullptr;
		for(; !bt_x.empty() && !bt_y.empty() && (bt_x.top() == bt_y.top());
			bt_x.pop(), bt_y.pop())
			z = bt_x.top();
		return z;
	}

private:
	const Node* m_p;
};

// ====================================
int main(int, char**)
{
	static const unsigned N = 127;
	srand(time(nullptr));

	// Gen unique numbers
	int nums[N];
	for(unsigned i = 0; i < N; i++)
	{
		int x;

		for(unsigned j = N; j != i;)
		{
			x = (rand() % 1000);// - 4096;
			for(j = 0; j < i; j++)
			{
				if(x == nums[j])
					break;
			}
		}

		nums[i] = x;
	}

	// Init tree
	Node* root = nullptr;
	for(unsigned i = 0; i < N; i++)
		insert(root, nullptr, nums[i]);

	// Get 2 random numbers for LCA
	unsigned i = rand() % N;
	unsigned j;
	do
		j = rand() % N;
	while(j == i);

	//std::cout << nums[i] << ":";
	std::cout << "Node A: " << nums[i];
	for(Node* p = find(root, nums[i])->p; p; p = p->p)
		std::cout << " <- " << p->N;
	std::cout << std::endl;

	std::cout << "Node B: " << nums[j];
	for(Node* p = find(root, nums[j])->p; p; p = p->p)
		std::cout << " <- " << p->N;
	std::cout << std::endl;

	// LCA using the parent pointer in nodes (simple)
	{
		LCA x(root, nums[i], nums[j], true);

		std::cout << "LCA (use parent pointer): ";
		if(const Node* lca = x.get())
			std::cout << lca->N << " @ level " << lca->L << std::endl;
		else
			std::cout << "not found" << std::endl;
	}
	// LCA without using the parent pointer in nodes
	{
		LCA x(root, nums[i], nums[j], false);

		std::cout << "LCA (don't use parent pointer): ";
		if(const Node* lca = x.get())
			std::cout << lca->N << " @ level " << lca->L << std::endl;
		else
			std::cout << "not found" << std::endl;
	}

	return 0;
}

