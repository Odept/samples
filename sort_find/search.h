#ifndef __SEARCH_H__
#define __SEARCH_H__


static const int& get_key(const int& item) { return item; }

// ====================================
// Binary search
template<class Key, class Item>
Item* find_binary(const Key& key, /*const*/ Item* a, unsigned n)
{
	if(!n)
		return NULL;

	// For interpolation search c = (key - a[0]) / (a[n - 1] - a[0]
	unsigned m = n / 2;
	Key k = get_key(a[m]);
	// 3-way partition style
	if(key == k)
	{
		// Return the first element in the range
		for(; m > 0 && get_key(a[m - 1]) == key; m--) {}
		return &a[m];
	}

	// Everything is ready for the next iteration if key < k
	if(key > k)
	{
		m++;
		a += m;
		m = n - m;
	}
	return find_binary(key, a, m);
}

// ====================================
// BST (Binary Search Tree)
template<class Key, class Item>
class BST
{
public:
	BST(Item* a, unsigned n):
		head(NULL)
	{
		for(unsigned i = 0; i < n; i++)
			insert(get_key(a[i]), a[i]);
	}

	void insert(const Key& key, Item& item)
	{
		if(!head)
		{
			head = new Node(key, item);
			return;
		}

		Node* p;
		unsigned c;
		for(p = head;; p = p->next[c])
		{
			const Key& k = get_key(*p->items[0]);
			if(key == k)
			{
				p->items.push_back(&item);
				return;
			}

			c = (key < k) ? Node::ChildLeft : Node::ChildRight;
			if(!p->next[c])
				break;
		}
		p->next[c] = new Node(key, item);
	}

	Item* find(const Key& key)
	{
		for(Node* c = head; c;)
		{
			const Key& k = get_key(*c->items[0]);
			if(key == k)
				return c->items[0];
			c = c->next[(key < k) ? Node::ChildLeft : Node::ChildRight];
		}
		return NULL;
	}

private:
	struct Node
	{
		enum {ChildLeft, ChildRight, ChildCount};
		Node(const Key& k, Item& item):
			key(k),
			items(1)
		{
			items[0] = &item;
			for(unsigned i = 0; i < ChildCount; i++)
				next[i] = NULL;
		}

		Key key;
		Node* next[ChildCount];
		std::vector<Item*> items;
	};

private:
	Node* head;
};

#endif // __SEARCH_H__

