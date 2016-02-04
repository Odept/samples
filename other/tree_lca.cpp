/**
 * Find the LCA (Lowest Common Ancestor) of given nodes in a binary tree.
 */
#include <iostream>
#include <iomanip>
#include <cstdlib>

#include <stack>


template<typename T>
class Tree
{
public:
	struct Node
	{
		Node *l, *r;
		Node *p;
		T N;
		unsigned L;
		Node(Node* _p, int x, unsigned lv): p(_p), N(x), L(lv), l(nullptr), r(nullptr) {}
	};

	// LCA functions
public:
	Node* lca(const T& x, const T& y, bool use_parent)
	{
		if(!m_root)
			return nullptr;

		if(use_parent)
			return traverse(find(x), find(y));
		else
		{
			Node* a = find(x);
			Node* b = find(y);
			if(!a || !b)
				return nullptr;
			return traverse(m_root, a, b);
		}
	}

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

	// Tree functions
public:
	Tree(): m_root(nullptr) {}

	void insert(const T& x) { insert(m_root, nullptr, x, 0); }
	Node* find(const T& x) { return find(m_root, x); }

private:
	void insert(Node*& p, Node* parent, const T& x, unsigned level)
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

	Node* find(Node* cur, int x)
	{
		if(!cur)
			return nullptr;

		if(cur->N == x)
			return cur;
		if(Node* l = find(cur->l, x))
			return l;
		if(Node* r = find(cur->r, x))
			return r;
		return nullptr;
	}

private:
	Node* m_root;
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
	Tree<int> tree;
	for(unsigned i = 0; i < N; i++)
		tree.insert(nums[i]);

	// Get 2 random numbers for LCA
	unsigned i = rand() % N;
	unsigned j;
	do
		j = rand() % N;
	while(j == i);

	//std::cout << nums[i] << ":";
	std::cout << "Node A: " << nums[i];
	for(auto p = tree.find(nums[i])->p; p; p = p->p)
		std::cout << " <- " << p->N;
	std::cout << std::endl;

	std::cout << "Node B: " << nums[j];
	for(auto p = tree.find(nums[j])->p; p; p = p->p)
		std::cout << " <- " << p->N;
	std::cout << std::endl;

	// LCA using the parent pointer in nodes (simple)
	{
		std::cout << "LCA (use parent pointer): ";
		if(auto lca = tree.lca(nums[i], nums[j], true))
			std::cout << lca->N << " @ level " << lca->L << std::endl;
		else
			std::cout << "not found" << std::endl;
	}
	// LCA without using the parent pointer in nodes
	{
		std::cout << "LCA (don't use parent pointer): ";
		if(auto lca = tree.lca(nums[i], nums[j], false))
			std::cout << lca->N << " @ level " << lca->L << std::endl;
		else
			std::cout << "not found" << std::endl;
	}

	return 0;
}

