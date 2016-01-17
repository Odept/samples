/**
 * Find the LCA (Lowest Common Ancestor) of given nodes in a binary tree.
 */
#include <iostream>
#include <iomanip>

#include <list>

// Node & Tree functions
struct Node
{
	Node *l, *r;
	Node *p;
	int N;
	unsigned L;
	Node(Node* _p, int x, unsigned lv): p(_p), N(x), L(lv), l(NULL), r(NULL) {}
};

static void insert(Node*& p, Node* parent, int x, unsigned level = 0)
{
	if(p)
	{
		if(arc4random() & 1)
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
		return NULL;

	if(p->N == x)
		return p;
	if(Node* l = find(p->l, x))
		return l;
	if(Node* r = find(p->r, x))
		return r;
	return NULL;
}

/*static void print(Node* p)
{
	if(!p)
		return;

	typedef std::list<Node*> fringe_t;
	fringe_t f;
	// BFS
	f.push_back(p);
	for(fringe_t::const_iterator it = f.begin(); it != f.end(); it++)
	{
		Node* p = *it;
		if(p->l)
			f.push_back(p->l);
		if(p->r)
			f.push_back(p->r);
	}
	// Print
	unsigned lv = 0;
	for(fringe_t::const_iterator it = f.begin(); it != f.end(); it++)
	{
		Node* p = *it;

		if(lv != p->L)
		{
			lv = p->L;
			std::cout << std::endl;
		}
		std::cout << std::setw(5) << p->N;
	}
	std::cout << std::endl;
}*/


// ====================================
class LCA
{
public:
	LCA(Node* root, int x, int y, bool use_parent):
		m_p(NULL)
	{
		if(!root)
			return;
		if(use_parent)
			traverse(find(root, x), find(root, y));
		else
			traverse(root, x, y);
	}

	const Node* get() const { return m_p; }

private:
	/**
	 * Solution for nodes without the parent pointer.
	 * Returns the number of hits for the given node (for internal use)
	 */
	unsigned traverse(const Node* p, int x, int y)
	{
		// Number of hits in the left child
		unsigned a = p->l ? traverse(p->l, x, y) : 0;
		if(a == 2)
			return a;

		// Number of hits in the right child
		unsigned b = p->r ? traverse(p->r, x, y) : 0;
		if(b == 2)
			return b;

		// Number of hits including the current node
		unsigned k = a + b + (p->N == x || p->N == y);
		if(k == 2)
			m_p = p;
		return k;
	}

	// Solution for nodes with the parent pointer
	void traverse(const Node* x, const Node* y)
	{
		// Get paths to root
		typedef std::list<const Node*> path_t;

		path_t path_x;
		for(const Node* p = x; p; p = p->p)
			path_x.push_front(p);
		path_t path_y;
		for(const Node* p = y; p; p = p->p)
			path_y.push_front(p);

		// Loop while paths are equal
		for(path_t::const_iterator itX = path_x.begin(), endX = path_x.end(),
								   itY = path_y.begin(), endY = path_y.end();
			itX != endX && itY != endY && *itX == *itY;
			itX++, itY++)
			m_p = *itX;
	}

private:
	const Node* m_p;
};

// ====================================
int main(int, char**)
{
	static const unsigned N = 127;

	// Gen unique numbers
	int nums[N];
	for(unsigned i = 0; i < N; i++)
	{
		int x;

		for(unsigned j = N; j != i;)
		{
			x = (arc4random() % 1000);// - 4096;
			for(j = 0; j < i; j++)
			{
				if(x == nums[j])
					break;
			}
		}

		nums[i] = x;
	}

	// Init tree
	Node* root;
	for(unsigned i = 0; i < N; i++)
		insert(root, NULL, nums[i]);

	// Print
	//print(root);

	// Get 2 random numbers for LCA
	unsigned i = arc4random() % N;
	unsigned j;
	do
	{
		j = arc4random() % N;
	}
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

