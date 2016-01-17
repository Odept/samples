/**
 * Given a binary tree, find its maximum depth.
 * The maximum depth of a binary tree is the number of NODES along the path
 * from the root node to the deepest leaf node.
 *
 * Note: use the BST for reference here
 */
#include <iostream>
#include <cmath>

#include <vector>
#include <set>


struct Node
{
	Node *l, *r;
	int N;
	unsigned L;
	Node(int x, unsigned lv): N(x), L(lv), l(NULL), r(NULL) {}
};

// ====================================
class BST
{
public:
	BST(): m_root(NULL) {}

	// Return false if already exists
	const Node* insert(int x) { return insert(m_root, x, 0); }

	unsigned depth() const { return depth(m_root); }
	//unsigned depth() const { return m_root ? depth(m_root) : 0; }

private:
	const Node* insert(Node*& node, int x, int level)
	{
		if(!node)
		{
			node = new Node(x, level);
			return node;
		}

		if(x == node->N)
			return NULL;
		else if(x < node->N)
			return insert(node->l, x, level + 1);
		else
			return insert(node->r, x, level + 1);
	}

	unsigned depth(const Node* node) const
	{
		if(!node)
			return 0;
		return std::max(depth(node->l), depth(node->r)) + 1;
//		unsigned dl = node->l ? (depth(node->l) + 1) : 0;
//		unsigned dr = node->r ? (depth(node->r) + 1) : 0;
//		return std::max(dl, dr);
	}

private:
	Node* m_root;
};

// ====================================
int main(int, char**)
{
	static const unsigned N = 511;

	// Gen unique numbers
	typedef std::set<int> numbers_t;
	
	numbers_t nums;
	do
	{
		int x = int(arc4random() % 1024) - 512;
		nums.insert(x);
	}
	while(nums.size() < N);

	// Get random root
	unsigned ir = arc4random() % N;

	numbers_t::const_iterator it = nums.begin();
	for(unsigned n = ir; n; n--, it++) {}
	int r = *it;

	// Random shuffle input to balance tree a bit (can't do it on an std::set)
	std::vector<int> vnums;
	for(numbers_t::const_iterator it = nums.begin(), end = nums.end(); it != end; it++)
		vnums.push_back(*it);

	std::random_shuffle(vnums.begin(), vnums.end());

	// Init tree
	BST bst;
	bst.insert(r);

	unsigned ref_depth = 0;
	for(unsigned i = 0; i < N; i++)
	{
		const Node* node = bst.insert(vnums[i]);
		if(node && node->L >= ref_depth)
			ref_depth = node->L + 1;
	}

	// Print Ref
	std::cout << "BST with root = " << r << " (" << ir << " / " << N << "), depth = " << ref_depth << " (min possible = " << unsigned(log(N) / log(2) + 1) << ")" << std::endl;

	// Calc & Print
	unsigned d = bst.depth();
	std::cout << "Calculated BST depth: " << d << std::endl;

	return 0;
}

