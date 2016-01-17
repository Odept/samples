/**
 * Check if there is a loop (cycle) in a linked list.
 * If there is one find the length and the starting point of the loop.
 *
 * Take two pointers “p1” and “p2”. p1 += 1, p2 += 2. If they meet, there is a loop.
 */
#include <iostream>


struct Entry
{
	Entry(int _x): x(_x), next(NULL) {}

	Entry* next;
	int x;
};

// ====================================
int main(int, char**)
{
	// Loop @
	const unsigned L = 32 + arc4random() % 32;
	// Loop to
	const unsigned D = 32 + arc4random() % 16;

	// Fill the list
	Entry* head = new Entry(0);

	Entry* loopTo = NULL;
	Entry* prev = head;
	for(unsigned i = 1; i < L; i++)
	{
		prev->next = new Entry(i);
		prev = prev->next;
		if(i == D)
			loopTo = prev;
	}
	prev->next = loopTo;

	// Print reference
	std::cout << "List of " << L << " elements ";
	if(prev->next)
		std::cout << "with cycle @ " << D << " (length = " << (L - D - 1) << ")";
	else
		std::cout << "without a cycle";
	std::cout << std::endl;

	// Find a loop: iterate the first pointer by 1, the second - by 2
	const Entry *p1, *p2;

	for(p1 = head ? head->next : NULL, p2 = (head && head->next) ? head->next->next : NULL;
		p1 && p2 && (p1 != p2);
		p1 = p1->next, p2 = (p2->next ? p2->next->next : NULL))
	{}

	// Get loop details if found
	std::cout << "Check: ";
	if(!p1 || !p2)
		std::cout << "no loop found" << std::endl;
	else
	{
		// Find the beginning of a loop: set one of the pointers to
		// the head and iterate till the pointers equal once again (?)
		for(p1 = head; p1 != p2; p1 = p1->next, p2 = p2->next) {}

		// Find the length
		unsigned l;
		for(l = 0, p2 = p2->next; p2 != p1; l++, p2 = p2->next) {}

		// Print
		std::cout << "found loop @ " << p1->x << " of length " << l << std::endl;
	}

	return 0;
}

