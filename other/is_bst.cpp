/**
 * Check whether given binary tree is BST or not.
 *
 * Solution is to do an in-order traversal, and check if it's sorted:
 *	1. Simple. Save elements to a vector during traversal and check if it's
 *	   sorted after that - Ospace(N);
 *	2. In-place. Maintain the latest traversed node and check for less-equal
 *	   in-place - Ospace(1).
 */
#include <iostream>

#include <vector>
#include <set>


struct Node
{
	Node *l, *r;
	int N;
	Node(int x): N(x), l(NULL), r(NULL) {}
};

// ====================================
class Tree
{
private:
	typedef std::vector<int> nodes_t;

public:
	Tree(): m_root(NULL) {}

	const Node* insert(int x) { return insert(m_root, x); }

	bool isBST(bool in_place) const
	{
		// Do the in-order traverse
		if(in_place)
		{
			int prev = std::numeric_limits<int>::min();
			return isBST(m_root, prev);
		}
		else
		{
			nodes_t nodes;
			inOrder(m_root, nodes);

			// Check if sorted
			unsigned n = nodes.size();
			if(n >= 2)
			{	
				int x = nodes[0];
				for(unsigned i = 1; i < n; i++)
				{
					if(x > nodes[i])
						return false;
					x = nodes[i];
				}
			}
			return true;
		}
	}

protected:
	virtual const Node* insert(Node*&, int x) = 0;

private:
	// In-place
	bool isBST(const Node* node, int& prev) const
	{
		if(!node)
			return true;
		// Left
		if(!isBST(node->l, prev))
			return false;
		// Current
		if(node->N < prev)
			return false;
		prev = node->N;
		// Right
		if(!isBST(node->r, prev))
			return false;
		return true;
	}

	void inOrder(const Node* node, nodes_t& nodes) const
	{
		if(!node)
			return;
		inOrder(node->l, nodes);
		nodes.push_back(node->N);
		inOrder(node->r, nodes);
	}

private:
	Node* m_root;
};

// ============================================================================
class BinaryTree : public Tree
{
private:
	const Node* insert(Node*& node, int x)
	{
		if(node)
		{
			if(arc4random() & 1)
				return insert(node->l, x);
			else
				return insert(node->r, x);
		}
		else
		{
			node = new Node(x);
			return node;
		}
	}
};


class BST : public Tree
{
private:
	const Node* insert(Node*& node, int x)
	{
		if(!node)
		{
			node = new Node(x);
			return node;
		}

		if(x == node->N)
			return NULL;
		else if(x < node->N)
			return insert(node->l, x);
		else
			return insert(node->r, x);
	}
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

	// Random shuffle input to balance tree a bit (can't do it on an std::set)
	std::vector<int> vnums;
	for(numbers_t::const_iterator it = nums.begin(), end = nums.end(); it != end; it++)
		vnums.push_back(*it);

	std::random_shuffle(vnums.begin(), vnums.end());

	// Init trees
	BinaryTree bt;
	BST bst;

	for(unsigned i = 0; i < N; i++)
	{
		// Fight name hiding
		bt .Tree::insert(vnums[i]);
		bst.Tree::insert(vnums[i]);
	}

	// Check if BST
	std::cout << "Trees inited with " << N << " random unique elements. Check if BST" << std::endl;

	std::cout << "\tBinary Tree: simple - " << (bt.isBST(false) ? "Yes" : "No") <<
				 ", in-place - " << (bt.isBST(true) ? "Yes" : "No") << std::endl;

	std::cout << "\tBST: simple - " << (bst.isBST(false) ? "Yes" : "No") <<
				 ", in-place - " << (bst.isBST(true) ? "Yes" : "No") << std::endl;

	return 0;
}

