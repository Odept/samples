/**
 * Count the number of power-of-2 substrings in the specified string
 * Time complexity: O(M^2), M = strlen
 */
#include <iostream>
#include <vector>

// Multiply a number string by 2
static void mul2(std::string& f_str)
{
	unsigned l = f_str.length();
	if(!l)
		return;

	unsigned carry = 0;
	for(int i = l - 1; i >= 0; i--)
	{
		unsigned d = f_str[i] - '0';
		carry += d * 2;
		f_str[i] = carry % 10 + '0';
		carry /= 10;
	}
	if(carry)
		f_str = std::string(1, '0' + carry) + f_str;
}

/**
 * 10-trie
 *
 * Prefixes are supported by marking an end-node as a leaf
 *
 * WARNING: strings are not checked for radix compliance
 */
class CTrie
{
private:
	static const unsigned R = 10;

	struct Node
	{
		Node* next[R];
		bool Leaf;
		Node(): Leaf(false) { for(unsigned i = 0; i < R; i++) next[i] = NULL; }
	};

public:
	~CTrie()
	{
		for(unsigned i = 0; i < R; i++)
		{
			if(m_root.next[i])
				free(m_root.next[i]);
		}
	}

	// Duplicates are automatically skipped
	void insert(const std::string& f_str)
	{
		if(unsigned l = f_str.length())
			insert(m_root.next[f_str[0] - '0'], f_str.c_str(), l - 1, 0);
	}

	// Return the number of prefix strings within the specified string
	unsigned substrings(const std::string& f_str, unsigned pos = 0) const
	{
		if(unsigned l = f_str.length() - pos)
			return substrings(m_root.next[f_str[pos] - '0'], f_str.c_str() + pos, l - 1, 0);
		else
			return 0;
	}

private:
	void insert(Node*& f_pNode, const char* f_str, unsigned f_last, unsigned f_d)
	{
		if(!f_pNode)
			f_pNode = new Node();

		if(f_d == f_last)
			f_pNode->Leaf = true;
		else
			insert(f_pNode->next[f_str[f_d + 1] - '0'], f_str, f_last, f_d + 1);
	}

	unsigned substrings(const Node* f_pNode, const char* f_str, unsigned f_last, unsigned f_d) const
	{
		if(!f_pNode)
			return 0;
		return ((f_pNode->Leaf ? 1 : 0) + 
				((f_d == f_last) ? 0 : substrings(f_pNode->next[f_str[f_d + 1] - '0'], f_str, f_last, f_d + 1)));
	}

	void free(Node*& f_pNode)
	{
		for(unsigned i = 0; i < R; i++)
		{
			if(f_pNode->next[i])
				free(f_pNode->next[i]);
		}
		delete f_pNode;
		f_pNode = NULL;
	}

private:
	Node m_root;
};

// ====================================
int main(int, char**)
{
	CTrie pow2trie;

	// Generate power-of-2 strings and add each "number" to the trie
	std::string pow2(1, '1');
	pow2trie.insert(pow2);

	static const unsigned power = 800;
	for(unsigned i = 0; i < power; i++)
	{
		mul2(pow2);
		pow2trie.insert(pow2);
	}

	// Input
	std::string x; // 2222222 24256 65536 023223 33579
	std::cout << "Number to check: "; std::cin >> x;

	// Calc
	unsigned n = 0;
	for(unsigned i = 0, l = x.length(); i < l; i++)
		n += pow2trie.substrings(x, i);
	
	// Output
	std::cout << "Number of power-of-2 substrings: " << n << std::endl;

	return 0;
}

