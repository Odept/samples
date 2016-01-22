#ifndef __SORT_H__
#define __SORT_H__


template<class Item>
static void exch(Item& a, Item& b)
{
	Item c = a;
	a = b;
	b = c;
}

// ====================================
// Selection Sort
template<class Item>
void sort_sel(Item* a, unsigned n)
{
	for(unsigned i = 0, r = n - 1; i < r; i++)
	{
		unsigned imin = i;
		Item min = a[imin];

		for(unsigned j = i + 1; j <= r; j++)
		{
			if(a[j] < min)
			{
				min = a[j];
				imin = j;
			}
		}
		exch(a[i], a[imin]);
	}
}

// ====================================
// Insertion Sort
template<class Item>
void sort_ins(Item* a, unsigned n)
{
	unsigned imin = 0;
	Item min = a[imin];
	// Preprocess: place min item at index 0
	for(unsigned i = 0 + 1; i < n; i++)
	{
		if(a[i] < min)
		{
			min = a[i];
			imin = i;
		}
	}
	exch(a[0], a[imin]);

	for(unsigned i = 0 + 2, j; i < n; i++)
	{
		Item cur = a[i];
		for(j = i; a[j - 1] > cur; j--)
			a[j] = a[j - 1];
		a[j] = cur;
	}
}

// ====================================
// Bubble Sort
template<class Item>
void sort_bub(Item* a, unsigned n)
{
	for(unsigned i = 0, r = n - 1; i < r; i++)
	{
		for(unsigned j = r; j > i; j--)
		{
			if(a[j - 1] > a[j])
				exch(a[j - 1], a[j]);
		}
	}
}

// ====================================
// Shell Sort
static int gap_seq(unsigned n, unsigned* seq = NULL)
{
	unsigned i = 0;

	for(unsigned h, n3 = (n < 15) ? 1 : (n / 3);;)
	{
		if(i & 1)
			h = (1 << (i + 3)) - 6 * (1 << ((i + 1) / 2)) + 1;
		else
			h = 9 * ((1 << i) - (1 << (i / 2))) + 1;
		if(h > n3)
			break;

		if(seq)
			seq[i] = h;
		i++;
	}

	return i;
}

template<class Item>
void sort_shell(Item* a, unsigned n)
{
	if(n < 2)
		return;

	unsigned nSteps = gap_seq(n);
	unsigned* steps = new unsigned[nSteps];
	gap_seq(n, steps);

	for(int istep = nSteps - 1; istep >= 0; istep--)
	{
		unsigned step = steps[istep];

		/**
		 * Loop over N/h steps in a step-sequence & h step-sequences
		 * simultaneously in a one loop (N/h * h = N loops) with a
		 * "for(i++)"
		 * instead of straightforward h loops of N/h loops such as
		 * "for(i < step; i++) for(j = i + step; j += step)"
		 */
		for(unsigned i = 0 + step, j; i < n; i++)
		{
			Item cur = a[i];
			for(j = i; (j >= 0 + step) && (a[j - step] > cur); j -= step)
				a[j] = a[j - step];
			a[j] = cur;
		}
	}

	delete[] steps;
}

// ====================================
// Quick Sort
template<class Item>
static unsigned partition(Item* a, unsigned n)
{
	unsigned l = n - 1;
	Item m = a[l];

	for(unsigned i = l - 1; i < n; i--)
	{
		if(a[i] >= m)
		{
			l--;
			exch(a[i], a[l]);
		}
	}
	if(l != n - 1)
		exch(a[l], a[n - 1]);

	return l;
}

template<class Item>
void sort_quick(Item* a, unsigned n)
{
	if(n < 2)
		return;
	unsigned i = partition(a, n);

	// Process the smaller part first to avoid deep recursion
	Item *a_small, *a_large;
	unsigned n_small, n_large;
	if(i < n / 2)	{ a_small = a; n_small = i;		a_large = a + i + 1; n_large = n - i - 1; }
	else			{ a_large = a; n_large = i;		a_small = a + i + 1; n_small = n - i - 1; }

	sort_quick(a_small, n_small);
	sort_quick(a_large, n_large);
}

// ====================================
// Quick Sort with a 3-way partition:
// {{less}, {eq}, {greater}}
struct PartitionBounds
{
	unsigned Equal;
	unsigned Greater;
	PartitionBounds(unsigned eq, unsigned gr): Equal(eq), Greater(gr) {}
};

template<class Item>
PartitionBounds partition3(Item* a, unsigned n)
{
	unsigned l = n - 1;
	unsigned mid = 0;
	Item m = a[l];

	// Place equal elements in the beginning in the first loop
	for(unsigned i = l - 1; i >= mid && i < n;)
	{
		if(a[i] == m)
		{
			exch(a[i], a[mid]);
			mid++;
			continue;
		}
		if(a[i] > m)
		{
			--l;
			exch(a[i], a[l]);
		}
		--i;
	}
	if(l != n - 1)
		exch(a[l], a[n - 1]);

	// Place equal elements in the middle of the array
	if(mid)
	{
		unsigned exchanges = l - mid;
		if(exchanges > mid)
			exchanges = mid;
		for(unsigned i = 0, j = l - 1; exchanges; i++, j--, exchanges--)
			exch(a[i], a[j]);
	}
	mid = l - mid;

	return PartitionBounds(mid, l + 1);
}

template<class Item>
static void sort_quick3(Item* a, unsigned n)
{
	if(n < 2)
		return;
	PartitionBounds bounds = partition3(a, n);

	// Process the smaller part first to avoid deep recursion
	Item *a_small, *a_large;
	unsigned n_small, n_large;

	n_small = bounds.Equal;
	n_large = n - bounds.Greater;
	if(n_small <= n_large)
	{
		a_small = a;
		a_large = a + bounds.Greater;
	}
	else
	{
		a_small = a + bounds.Greater;
		n_small = n - bounds.Greater;
		a_large = a;
		n_large = bounds.Equal;
	}

	sort_quick3(a_small, n_small);
	sort_quick3(a_large, n_large);
}

// ====================================
// Merge Sort
template<class Item>
static void merge(Item* dst, Item* src, unsigned n2, unsigned n)
{
	unsigned i, j, k;
	for(i = 0, j = n2, k = 0; i < n2 && j < n; k++)
	{
		if(src[i] < src[j])
			dst[k] = src[i++];
		else
			dst[k] = src[j++];
	}
	while(i < n2)
		dst[k++] = src[i++];
	while(j < n)
		dst[k++] = src[j++];

}

template<class Item>
static void sort_merge_internal(Item* a, Item* aux, unsigned n)
{
	if(n == 1)
		return;
	unsigned n2 = n / 2;
	sort_merge_internal(aux, a, n2);
	sort_merge_internal(aux + n2, a + n2, n - n2);
	merge(a, aux, n2, n);
}

template<class Item>
void sort_merge(Item* a, unsigned n)
{
	if(n < 2)
		return;

	Item* aux = new Item[n];
	for(unsigned i = 0; i < n; i++)
		aux[i] = a[0 + i];

	sort_merge_internal(a + 0, aux, n);

	delete aux;
}

// ============================================================================
template<class Item>
static bool is_sorted(Item* a, unsigned n)
{
	if(n < 2)
		return true;

	for(unsigned i = 1; i < n; i++)
	{
		if(a[i - 1] > a[i])
			return false;
	}

	return true;
}

#endif // __SORT_H__

