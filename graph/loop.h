#ifndef __GRAPH_LOOP__
#define __GRAPH_LOOP__

#include "traverse.h"
#include "path.h"


class CGraphLoop
{
public:
	typedef std::vector<unsigned> loop_t;

public:
	template<class G>
	CGraphLoop(const G& f_g)
	{
		unsigned V = f_g.V(), vv = 0;
		if(V < 2)
			return;

		CGraphDFS<G> dfs(f_g);
		LoopInfo info(V);

		for(unsigned i = 0; i < V; ++i)
		{
			if(dfs.visited(i))
				continue;

			dfs.traverse(i, cb, &info);
			if(info.Visited == V)
				break;
		}
		ASSERT(info.Visited == V);

		// Process back-edges
		unsigned n = dfs.back_count();
		if(!n)
			return;

		m_loops.resize(n);
		for(unsigned i = 0; i < n; i++)
		{
			loop_t& l = m_loops[i];

			const edge_t& e = dfs.back(i);
			unsigned top = e.second;
			unsigned bottom = e.first;

			// Find the shortest path without the back-edge
			CGraphPath<G> path(f_g, e);
			path.traverse(top);

			for(unsigned v = bottom; v != top; v = path.parent(v))
			{
				ASSERT(path.visited(v));
				l.push_back(v);
			}
			l.push_back(top);
			l.push_back(bottom);
		}
	}

	unsigned count() const { return m_loops.size(); }
	const loop_t& loop(unsigned f_i) { return m_loops[f_i]; }

private:
	struct LoopInfo
	{
		unsigned Visited;
		LoopInfo(unsigned f_V): Visited(0) {}
	};

	static bool cb(void* f_param, unsigned f_p, unsigned f_v)
	{
		LoopInfo& info = *reinterpret_cast<LoopInfo*>(f_param);
		info.Visited++;
		return true;
	}

private:
	std::vector<loop_t> m_loops;
};

#endif // __GRAPH_LOOP__

