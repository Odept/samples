#ifndef __GRAPH_TRAVERSE__
#define __GRAPH_TRAVERSE__

#include <queue>
#include <list>


typedef bool (*traverse_cb_t)(void* f_param, unsigned f_v, unsigned f_w);

typedef std::pair<unsigned, unsigned> edge_t;

// ============================================================================
// Generic traverse scheme
template<class G, class FRINGE>
class CGraphTraverse
{
public:
	void traverse(unsigned f_v, traverse_cb_t f_cb = NULL, void* f_param = NULL)
	{
		if(f_v >= m_g.V() || m_parent[f_v])
			return;

		FRINGE s;
		push_if(s, f_v, f_v);

		for(unsigned pre = 0; !s.empty(); pre++)
		{
			// Pop
			edge_t e = pop(s);
			unsigned w = e.second;

			// Order
			m_pre[pre] = w;

			// Visit
			unsigned parent = e.first;
			bool cont = f_cb ? f_cb(f_param, parent, w) : true;
			m_parent[w] = parent + 1;
			if(!cont)
				break;

			// Handle linked
			for(typename G::AdjIterator it = m_g.begin(w), end = m_g.end(w); it != end; ++it)
				push_if(s, w, *it);
		}
	}

	//unsigned pre_order(unsigned f_at) const { return m_pre.at(f_at); }
	unsigned parent(unsigned f_v) const { return m_parent.at(f_v) - 1; }
	bool visited(unsigned f_v) const { return m_parent.at(f_v); }

protected:
	CGraphTraverse(const G& f_g): m_g(f_g), m_pre(f_g.V()), m_parent(f_g.V()), m_queued(f_g.V()) {}
	virtual ~CGraphTraverse() {}

	virtual void	push_if(FRINGE& f_s, unsigned f_v, unsigned f_w) = 0;
	virtual edge_t	pop    (FRINGE& f_s) = 0;

protected:
	const G& m_g;

	std::vector<unsigned> m_pre;
	std::vector<unsigned> m_parent;
	std::vector<bool> m_queued;
};

// ============================================================================
// DFS (fringe = stack)
typedef std::list<edge_t> fringe_dfs_t;

template<class G>
class CGraphDFS : public CGraphTraverse<G, fringe_dfs_t>
{
public:
	CGraphDFS(const G& f_g): CGraphTraverse<G, fringe_dfs_t>(f_g), m_it(f_g.V()) {}
	virtual ~CGraphDFS() {}

	unsigned back_count() const { return m_back.size(); }
	const edge_t& back(unsigned f_i) const { return m_back.at(f_i); }

protected:
	virtual void push_if(fringe_dfs_t& f_s, unsigned f_v, unsigned f_w)
	{
		// Check if visited (parent-edges/back-edges?)
		if(this->visited(f_w))
		{
			if(this->parent(f_v) != f_w)
				m_back.push_back( edge_t(f_v, f_w) );
			return;
		}
		// Delete previous item in "stack" if was pushed
		if(this->m_queued[f_w])
			f_s.erase(m_it[f_w]);
		f_s.push_front( edge_t(f_v, f_w) );
		m_it[f_w] = f_s.begin();
		this->m_queued[f_w] = true;
	}
	virtual edge_t pop(fringe_dfs_t& f_s)
	{
		edge_t e = f_s.front();
		f_s.pop_front();
		return e;
	}

private:
	std::vector<fringe_dfs_t::iterator> m_it;
	std::vector<edge_t> m_back;
};

// ============================================================================
// BFS (fringe = queue)
typedef std::queue<edge_t> fringe_bfs_t;

template<class G>
class CGraphBFS : public CGraphTraverse<G, fringe_bfs_t>
{
public:
	CGraphBFS(const G& f_g): CGraphTraverse<G, fringe_bfs_t>(f_g) {}
	virtual ~CGraphBFS() {}

protected:
	virtual void push_if(fringe_bfs_t& f_s, unsigned f_v, unsigned f_w)
	{
		if(!this->m_queued[f_w])
		{
			f_s.push( edge_t(f_v, f_w) );
			this->m_queued[f_w] = true;
		}
	}
	virtual edge_t pop(fringe_bfs_t& f_s)
	{
		edge_t e = f_s.front();
		f_s.pop();
		return e;
	}
};

#endif // __GRAPH_TRAVERSE__

