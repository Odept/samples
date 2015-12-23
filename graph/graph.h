#ifndef __GRAPH_BASE__
#define __GRAPH_BASE__

#include <vector>
#include <ostream>

#include <cstdlib>
#define ASSERT(cond) if(!(cond)) abort()


class CGraph
{
public:
	CGraph(unsigned f_V): m_E(0), m_degree(f_V) {}

	unsigned V() const { return m_degree.size(); }
	unsigned E() const { return m_E; }
	unsigned degree(unsigned f_v) const { return (f_v < V() ? m_degree[f_v] : 0 ); }

	friend std::ostream& operator<<(std::ostream& f_os, const CGraph& f_g) { f_g.print(f_os); return f_os; }

protected:
	virtual void print(std::ostream& f_os) const = 0;

protected:
	unsigned m_E;
	std::vector<unsigned> m_degree;
};


class CVGraph : public CGraph
{
public:
	CVGraph(unsigned f_V): CGraph(f_V), m_matrix(f_V * f_V) {}

	void insert(unsigned f_v, unsigned f_w)
	{
		unsigned n = V();
		if(f_v >= n || f_w >= n)
			return;
		set(f_v, f_w, n); m_degree[f_v]++;
		set(f_w, f_v, n); m_degree[f_w]++;
		m_E++;
	}

protected:
	void print(std::ostream& f_os) const
	{
		for(unsigned i = 0, n = V(); i < n; i++)
		{
			for(unsigned j = 0; j < n; j++)
				f_os << get(i, j, n) << ' ';
			f_os << std::endl;
		}
	}

private:
	void set(unsigned f_v, unsigned f_w, unsigned f_n)       { m_matrix[f_v * f_n + f_w] = true; }
	bool get(unsigned f_v, unsigned f_w, unsigned f_n) const { return m_matrix[f_v * f_n + f_w]; }
	
private:
	std::vector<bool> m_matrix;
};

// ============================================================================
// Adjacency List Graph
struct Edge
{
	Edge(): prev(this), next(this), pair(NULL), Vertex(NullVertex) {}
	Edge(unsigned f_v, Edge* f_prev):
		prev(f_prev), next(f_prev->next), pair(NULL), Vertex(f_v)
		//next(f_next), prev(f_next->prev), pair(NULL), Vertex(f_v)
	{ next->prev = prev->next = this; }

	void link(Edge* f_v)
	{
		ASSERT(this != f_v);
		pair = f_v;
		f_v->pair = this;
	}

	Edge* prev;
	Edge* next;
	Edge* pair;
	unsigned Vertex;

	static const unsigned NullVertex = ~(unsigned)0;
};


class CEGraph : public CGraph
{
	// Iterator
public:
	class AdjIterator
	{
		friend class CEGraph;
	public:
		AdjIterator& operator++()
		{
			if(m_cur->Vertex != Edge::NullVertex)
				m_cur = m_cur->next;
			return *this;
		}
		unsigned operator*() const { return m_cur->Vertex; }
		bool operator!=(const AdjIterator& f_it) const { return (m_cur != f_it.m_cur); }
		bool operator==(const AdjIterator& f_it) const { return (m_cur == f_it.m_cur); }
	private:
		AdjIterator(): m_cur(&m_dummy) {}
		AdjIterator(Edge* f_p): m_cur(f_p) {}
	private:
		Edge* m_cur;
		static Edge m_dummy;
	};

	AdjIterator begin(unsigned f_v) const { return AdjIterator((f_v < V()) ? m_vertices[f_v]->next : NULL); }
	AdjIterator   end(unsigned f_v) const { return AdjIterator((f_v < V()) ? m_vertices[f_v]       : NULL); }

	// ================================
public:
	CEGraph(unsigned f_V):
		CGraph(f_V),
		m_vertices(f_V)
	{ init(); }
	CEGraph(const CEGraph& f_g):
		CGraph(f_g.V()),
		m_vertices(f_g.V())
	{
		init();
		for(unsigned v = 0, n = f_g.V(); v < n; v++)
		{
			for(AdjIterator it = f_g.begin(v), end = f_g.end(v); it != end; ++it)
			{
				if(v < *it)
					insert(v, *it);
			}
		}
	}
	~CEGraph()
	{
		for(unsigned v = 0, n = V(); v < n; v++)
		{
			Edge* f = m_vertices[v];
			for(Edge* p = f->next; p != f;)
			{
				Edge* c = p;
				p = p->next;
				delete c;
			}
			delete f;
		}
	}

	void insert(unsigned f_v, unsigned f_w)
	{
		unsigned n = V();
		if(f_v >= n || f_w >= n)
			return;
		Edge* a = ins(f_v, f_w);
		Edge* b = ins(f_w, f_v);
		a->link(b);
		m_E++;
	}
	void remove(AdjIterator& f_it)
	{
		m_degree[f_it.m_cur->Vertex]--;
		m_degree[f_it.m_cur->pair->Vertex]--;
		rem(f_it.m_cur->pair);
		rem(f_it.m_cur);
		m_E--;
	}

private:
	void init()
	{
		for(unsigned v = 0, n = V(); v < n; v++)
			m_vertices[v] = new Edge();
	}
	Edge* ins(unsigned f_v, unsigned f_w)
	{
		Edge* e = new Edge(f_w, m_vertices[f_v]->prev);
		m_degree[f_v]++;
		return e;
	}
	void rem(Edge* f_p)
	{
		f_p->prev->next = f_p->next;
		f_p->next->prev = f_p->prev;
		delete f_p;
	}

	void print(std::ostream& f_os) const
	{
		for(unsigned v = 0, n = V(); v < n; v++)
		{
			f_os << v << ':';
			for(AdjIterator it = begin(v), end = this->end(v); it != end; ++it)
				f_os << ' ' << *it;
			f_os << std::endl;
		}
	}
	
private:
	// vector of heads of adjacency lists
	std::vector<Edge*> m_vertices;
};

#endif // __GRAPH_BASE__

