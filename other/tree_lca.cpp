/**
 * Find the LCA (Lowest Common Ancestor) of given nodes in a binary tree.
 *
 * 3 solutions:
 *	- use a link to the parent to compare backtraces
 *	- use nodes' level during in-order traversal between nodes
 *	- don't use any additional info
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

	// ================================
	// LCA functions
public:
	Node* lca_with_parent(const T& x, const T& y) { return lca(x, y); }
	Node* lca_robust(const T& x, const T& y)
	{
		Node* a = find(x);
		Node* b = find(y);
		return (a && b) ? lca(m_root, *a, *b) : nullptr;
	}
	Node* lca_with_level(const T& x, const T& y)
	{
		Node* lowest = NULL;
		return lca(m_root, x, y, lowest);
	}

private:
	/**
	 * The most strict solution when only left and right pointers are
	 * available. Requires that two nodes exist in the tree.
	 */
	Node* lca(Node* cur, const Node& x, const Node& y)
	{
		if(!cur)
			return nullptr;

		if(cur == &x || cur == &y)
			return cur;

		Node* pR = lca(cur->l, x, y);
		Node* pL = lca(cur->r, x, y);

		if(pR && pL)
			return cur;
		return pR ? pR : pL;
	}

	/**
	 * Get "backtraces" to the root and return the last node common for both
	 * backtraces. Requires a pointer to the parent, but can be easily used
	 * for LCA of multiple nodes.
	 */
	Node* lca(const T& _x, const T& _y)
	{
		typedef std::stack<Node*> backtrace_t;

		Node* x = find(_x);
		Node* y = find(_y);
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

	/**
	 * In-post-order traverse the path between two nodes and return a node
	 * closest to the root. Uses level information for the traversal, but
	 * doesn't require nodes to be present in a tree.
	 */
	Node* lca(Node* cur, const T& x, const T& y, Node*& lowest)
	{
		if(!cur)
			return nullptr;

		// Left
		Node* p = lca(cur->l, x, y, lowest);
		if(p)
			return p;

		// Hit?
		if(cur->N == x || cur->N == y)
		{
			// Second hit = found
			if(lowest)
				return ((cur->L < lowest->L) ? cur : lowest);
			// First hit
			lowest = cur;
		}
		// Looking for the second hit?
		else if(lowest && cur->L < lowest->L)
			lowest = cur;

		return lca(cur->r, x, y, lowest);
	}

	// ================================
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

	friend std::ostream& operator<<(std::ostream& os, const Node& x)
	{
		std::stack<const Node*> s;
		for(const Node* p = &x; p; p = p->p)
			s.push(p);
		os << s.top()->N;
		for(s.pop(); !s.empty(); s.pop())
			os << " -> " << s.top()->N;
		return os;
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
	std::cout << "Node A: ";
	auto a = tree.find(nums[i]);
	if(a)
		std::cout << *a;
	else
		std::cout << "none";
	std::cout << std::endl;

	std::cout << "Node B: ";
	auto b = tree.find(nums[j]);
	if(b)
		std::cout << *b;
	else
		std::cout << "none";
	std::cout << std::endl;

	// LCA using the parent pointer in nodes
	{
		std::cout << "LCA (use parent pointer)  : ";
		if(auto lca = tree.lca_with_parent(nums[i], nums[j]))
			std::cout << lca->N << " @ level " << lca->L << std::endl;
		else
			std::cout << "not found" << std::endl;
	}
	// LCA w/o using any additinal information from the nodes
	{
		std::cout << "LCA (don't use extra info): ";
		if(auto lca = tree.lca_robust(nums[i], nums[j]))
			std::cout << lca->N << " @ level " << lca->L << std::endl;
		else
			std::cout << "not found" << std::endl;
	}
	// LCA using the nodes' level information
	{
		std::cout << "LCA (use level info)      : ";
		if(auto lca = tree.lca_with_level(nums[i], nums[j]))
			std::cout << lca->N << " @ level " << lca->L << std::endl;
		else
			std::cout << "not found" << std::endl;
	}

	return 0;
}

